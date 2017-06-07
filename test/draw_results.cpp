#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Chi2Test

// -- ROOT headers --
#include <TApplication.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TAxis.h>
#include <TF1.h>


#include <boost/test/unit_test.hpp>
#include <iostream>

// -- User headers --
#include "PhysicalProcess.h"
#include "FitManager.h"

#include <mpi.h>



namespace tt = boost::test_tools;

using std::cout;
using std::endl;

class Plotter
{
public:
    Plotter():
        fManager(FitManager::GetFitManager()),
        fApp(new TApplication("ProFit", 0, 0)),
        fEnergy_ds_pbp(53.018),
        fEnergy_ds_pp(44.7)
    {
        fFitFunction = TF1("fFitFunction", fManager.GetModel(), &TheoreticalModel::DrawFunction, 20, 3000, fManager.GetModel()->npars + 2);
        PhysicalProcess input_array[] =
        {
            PhysicalProcess("sigma_pp",    110, "#sigma_{pp}"),
            PhysicalProcess("sigma_pbarp", 111, "#sigma_{p#bar{p}}"),
            PhysicalProcess("rho_pp",      210, "#rho_{pp}"),
            PhysicalProcess("rho_pbarp",   211, "#rho_{p#bar{p}}"),
            PhysicalProcess("ds_pp",       310, "#frac{d#sigma}{dt}_{pp}"),
            PhysicalProcess("ds_pbarp",    311, "#frac{d#sigma}{dt}_{p#bar{p}}")


        };
        std::vector<PhysicalProcess> input_vector(input_array, input_array + sizeof(input_array) / sizeof(PhysicalProcess));
        fManager.GetData("../Data.root", input_vector);

        // NB: Here use real parameters
        fManager.GetParameters("../parameters.in");
        SetFitParameters();

        BOOST_TEST_MESSAGE("Setup the plotter");
    }

    ~Plotter()
    {
        fApp->Run();
        BOOST_TEST_MESSAGE("Teardown the plotter");
        delete fApp;
    }

    void DrawResults()
    {
        const FitManager::DataSet & data = fManager.Data();
        TCanvas * canvas = new TCanvas("Main Canvas", "experimental data", 800, 600);
        canvas->Divide(2, 3);

        for (int i = 0; i < data.size(); ++i)
        {
            TVirtualPad * pad = canvas->cd(i + 1);
            pad->cd();
            DrawGraph(data[i]);
            DecoratePad(pad, data[i].dataCode);
        }
        canvas->Show();
        canvas->SaveAs(TString::Format("plots_with_pp%.2g_pap_%.2g_.png", fEnergy_ds_pp, fEnergy_ds_pbp));
    }
private:
    FitManager & fManager;
    TApplication * fApp;
    TF1 fFitFunction;
    float fEnergy_ds_pp;
    float fEnergy_ds_pbp;


    void DecoratePad(TVirtualPad * pad, int code)
    {
        pad->cd();
        pad->SetTickx();
        pad->SetTicky();
        pad->SetGridx();
        pad->SetGridy();

        TLatex energy_label;
        energy_label.SetTextFont(43);
        energy_label.SetTextSize(20);

        if (code == 310)
            energy_label.DrawText(0.1, 0.5, TString::Format("#sqrt{s} = %.2g", fEnergy_ds_pp));

        if (code == 311)
            energy_label.DrawText(0.5, 0.5, TString::Format("#sqrt{s} = %.2g", fEnergy_ds_pbp));

        if (code < 300)
            pad->SetLogx();

        if (code > 300)
            pad->SetLogy();
    }


    TGraphErrors * DrawGraph(const PhysicalProcess& proc)
    {
        TGraphErrors  graph = TGraphErrors();
        graph.SetName(proc.name);
        graph.SetTitle(proc.title);

        int npoints = 0;
        for (int i = 0; i < proc.numberOfpoints; ++i)
        {
            DataPoint point = proc.experimentalPoints[i];
            // here draw cuts should be applied
            if (proc.dataCode == 310 && int(point.energy) != int(fEnergy_ds_pp))
                continue;

            if (proc.dataCode == 311 && int(point.energy) != int(fEnergy_ds_pbp))
                continue;

            double x = (proc.dataCode  < 300) ? point.energy : point.t;
            double y = point.observable;
            double dy = point.error;

            graph.SetPoint(npoints, x, y);
            graph.SetPointError(npoints, 0, dy);
            ++npoints;
        }

        graph.GetXaxis()->SetTitle((proc.dataCode  < 300) ? "#sqrt{s}, GeV" : "|t|, GeV^{2}");
        // TODO: add units to y-title
        graph.GetYaxis()->SetTitle(TString(proc.title));
        graph.SetMarkerStyle(20);
        graph.SetMarkerColor(46);
        graph.DrawClone("AP");
        DrawFitFunction(proc);
    }


    void DrawFitFunction(const PhysicalProcess& proc)
    {
        int shift = 2;
        if (proc.dataCode / 100 == 3)
        {
            fFitFunction.SetRange(0.1 , 10);
            fFitFunction.SetParameter(0, proc.dataCode % 10 ? fEnergy_ds_pbp : fEnergy_ds_pp);
        }
        else
        {
            fFitFunction.SetRange(5, 3e+5);
            fFitFunction.SetParameter(0, 0); // t = 0
        }

        fFitFunction.SetParameter(1, proc.dataCode);
        fFitFunction.DrawClone("same");  // these 2 lines needs to be run together!!
    }

    void SetFitParameters()
    {
        float * pars;
        int npars;
        fManager.GetParameters(pars, npars);

        for (int i = 0; i < npars; ++i)
            fFitFunction.SetParameter(i, pars[i]);
        // delete [] pars;
    }

};

BOOST_FIXTURE_TEST_SUITE(PlottingTheResults, Plotter)

BOOST_AUTO_TEST_CASE(Plot)
{

    DrawResults();
    BOOST_TEST_MESSAGE("TODO: Implement plotting here.");
}


BOOST_AUTO_TEST_SUITE_END()
