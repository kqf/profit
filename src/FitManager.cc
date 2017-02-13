/*
 * File:   FitManager.cc
 * Author: sha
 *
 * Created on May 4, 2014, 8:56 AM
 */

#include "FitManager.h"

#include <TChain.h>
#include <TF1.h>
#include <TAxis.h>
#include <TString.h>
#include <TLatex.h>


#include <cassert>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <omp.h>


FitManager * FitManager::_instance = 0;

FitManager::FitManager():
	graphs(0),
	main_canvas(0),
	gMinimizer(0),
	fitFunction(0)
{
	ds_pbp_energy = 53.018;
	ds_pp_energy = 44.7;
}

FitManager & FitManager::GetFitManager()
{
	if (_instance == 0) _instance =  new FitManager();
	return * _instance;
}

void FitManager::GetData(const char * filename, std::vector<PhysicalProcess> input)
{
	processes = input;
	root_file = filename;

	// foreach does not plays well with meber function parameters as refs.
	for (int i = 0; i < processes.size(); ++i)
		FillProcess(processes[i]);
}

bool FitManager::Cut(const DataPoint& p, int procType)
{
	double lower_t = 0.01;
	double upper_t = 9;
	double lower_energy = 19;
	double upper_energy = 2000;

	double lower_energy_sigma = 5;
	double upper_energy_sigma = 2000;

	int exclude_energy = ConvertEnergy(52.818);

	double aexclude = 52.79;
	double bexclude = 52.85;

	bool is_valid = false;
	if (procType > 300)
		is_valid  = ( (p.energy > lower_energy) && (p.energy < upper_energy) ) &&
		            ( (p.t > lower_t) && (p.t < upper_t));
	else
		is_valid =  ( (p.energy > lower_energy_sigma) && (p.energy < upper_energy_sigma) ) ;

	//    if (ConvertEnergy(p.energy) == exclude_energy)
	//	return false;

	if ( ( (p.energy > aexclude) && (p.energy < bexclude) ) && procType == 310 )
		return true;

	// true include data point, false exclude
	return !is_valid;
}

void FitManager::FillProcess(PhysicalProcess & proc)
{
	// TODO: rewrite this
	// this should be a static method of PhysicalProcess
	// something like:
	// std::vector<PhysicalProcess> PhysicalProcess::CreateProcessesFromFile("Data.root")

	TChain * dat = new TChain(proc.name);
	dat->Add(root_file);
	float a, b , c, d;

	dat ->SetBranchAddress("intensity", &a);
	dat ->SetBranchAddress("error", &b);
	dat ->SetBranchAddress("energy", &c);
	dat ->SetBranchAddress("t", &d);
	for (int i = 0 ; i < dat->GetEntries(); ++i )
	{
		dat->GetEntry(i);
		proc.experimentalPoints.push_back(DataPoint((double)a, (double)b, (double)c, (double)d));

		DataPoint & point = proc.experimentalPoints[i];
		if ( Cut(point, proc.dataCode) )
			point.ignore = true;
	}
	proc.numberOfpoints = proc.experimentalPoints.size();

	std::cout << "Number of points for " << proc.name << " : " <<  proc.experimentalPoints.size() << std::endl;

	delete dat;
	dat = 0;
}

void FitManager::GetParameters(const char * filename)
{
	fit_parameters = ModelParameter::GetParameters(filename);

	std::copy(fit_parameters.begin(), fit_parameters.end(), std::ostream_iterator<ModelParameter>(std::cout, ""));

	const int inp_length = fit_parameters.size();
	double values[inp_length];

	for (int i = 0; i < fit_parameters.size(); ++i)
		values[i] = fit_parameters[i].value;

	currentModel = TheoreticalModel(values, inp_length);

	fitFunction = new TF1("fitFunction", &currentModel, &TheoreticalModel::DrawFunction, 2, 30000, currentModel.npars + 2);
	fitFunction->SetLineColor(37);
}

void FitManager::DrawApproximation()
{
	if (!main_canvas)
		main_canvas = new TCanvas("Main Canvas", "experimental data", 800, 600);
	main_canvas->Divide(2, 3);
	for (int i = 0; i < processes.size(); ++i)
		CreateGraph(processes[i]);


	TLatex energy_label;
	energy_label.SetTextFont(43);
	energy_label.SetTextSize(20);
	for (int i = 0; i < graphs.size(); ++i)
	{
		main_canvas->cd(i + 1);
		if (processes[i].dataCode < 300)
			gPad->SetLogx();

		if (processes[i].dataCode > 300)
			gPad->SetLogy();

		graphs[i]->Draw("AP");
		DrawFitFunction(processes[i]);

		// if(processes[i].dataCode == 310)
		energy_label.DrawText(0.1, 0.5, TString::Format("#sqrt{s} = %.2g", ds_pp_energy));

		// if(processes[i].dataCode == 311)
		// energy_label.DrawText(0.5, 0.5, TString::Format("#sqrt{s} = %.2g", ds_pbp_energy));

		graphs[i]->Draw("AP");
		DrawFitFunction(processes[i]);
	}
	main_canvas->Show();
	main_canvas->SaveAs(TString::Format("plots_with_pp%.2g_pap_%.2g_.png", ds_pp_energy, ds_pbp_energy));
}

void FitManager::CreateGraph(PhysicalProcess& proc)
{
	TGraphErrors * graph = new TGraphErrors();
	graph->SetName(proc.name);
	graph->SetTitle(proc.title);

	//    TODO: Check wheather numberOfpoints is needed
	int npoints = 0;
	for (int i = 0; i < proc.numberOfpoints; ++i)
	{
		DataPoint point = proc.experimentalPoints[i];
		// here draw cuts should be applied
		if (proc.dataCode == 310 && int(point.energy) != int(ds_pp_energy))
			continue;

		if (proc.dataCode == 311 && int(point.energy) != int(ds_pbp_energy))
			continue;

		double x = (proc.dataCode  < 300) ? point.energy : point.t;
		double y = point.observable;
		double dy = point.error;

		graph ->SetPoint(npoints, x, y);
		graph ->SetPointError(npoints, 0, dy);
		++npoints;
	}

	graph->GetXaxis()->SetTitle((proc.dataCode  < 300) ? "#sqrt{s}, GeV" : "|t|, GeV^{2}");
	// TODO: add units to y-title
	graph->GetYaxis()->SetTitle(TString(proc.title));
	graph->SetMarkerStyle(20);
	graph->SetMarkerColor(46);

	graphs.push_back(graph);
}

void FitManager::DrawFitFunction(PhysicalProcess& proc)
{
	assert(fitFunction != 0);

	int shift = 2;
	if (proc.dataCode / 100 == 3)
	{
		fitFunction->SetRange(0.1 , 10);
		fitFunction->SetParameter(0, proc.dataCode % 10 ? ds_pbp_energy : ds_pp_energy);
	}
	else
	{
		fitFunction->SetRange(5, 3e+5);
		fitFunction->SetParameter(0, 0);  // t = 0
	}

	fitFunction->SetParameter(1, proc.dataCode);

	for (int i = 0; i < fit_parameters.size(); ++i)
		fitFunction->FixParameter(i + shift, fit_parameters[i].value);

	fitFunction->Draw("same");  // these 2 linesk needs to be run together!!
	main_canvas->Update();
}

static void fcn(int& npar, double* gradients, double& f, double* p, int flag)
{
	FitManager & manager = FitManager::GetFitManager();
	f = manager.chi2(p);
}


void FitManager::SetupMinimizer()
{
	int npars = fit_parameters.size();

	assert(npars != 0 && "Cannot initialize minimizer with 0 input parameters");
	gMinimizer = new TMinuit(npars);
	gMinimizer->SetFCN(fcn);

	//    TODO: Implement parameters properties
	double arglist[10];
	int ierflg = 0;

	arglist[0] = 1;
	gMinimizer->mnexcm("SET ERR", arglist, 1, ierflg);

	for (int i = 0; i < fit_parameters.size(); ++i)
		gMinimizer->mnparm(i
		                   , fit_parameters[i].name
		                   , fit_parameters[i].value
		                   , fit_parameters[i].step_size
		                   , fit_parameters[i].lower_bound
		                   , fit_parameters[i].upper_bound
		                   , ierflg);
}

// TODO:: No default values here
double FitManager::chi2(const double * parameters = 0)
{
	//static int i = 0;
	//++i;
	//std::cout << "Calling fcn for "  << i << " times" << std::endl;
	if (parameters != 0)
		currentModel.SetParameters(parameters);


	double result = 0;
	double chi2_per_process = 0;
	for (int i = 0; i < processes.size() ; ++i )
	{
		currentModel.SetProcessType(processes[i].dataCode);
		// std::cout << "Processing " << processes[i].dataCode << std::endl;
		chi2_per_process = 0;
		TheoreticalModel computor(currentModel);

		int npoints = processes[i].numberOfpoints;
		#pragma omp parallel for firstprivate(computor) reduction(+:chi2_per_process)
		for (int j = 0;  j <  npoints; ++j)
		{
			const DataPoint & p = processes[i].experimentalPoints[j];
			// if(p.ignore) continue;

			// std::cout << " Calculating >> " << std::endl;
			double y =  computor.GetTheoreticalValue(p.energy, p.t);
			double delta =  (p.observable - y) / p.error ;


			// std::cout << std::setw(8) << p.energy << "\t"
			//              << std::setw(8) << p.t << "\t"
			//              << std::setw(8) << p.observable << "\t"
			//              << std::setw(8) << y << "\terror\t"
			//              << std::setw(8) << p.error << "\t"
			// << processes[i].dataCode << "\t"
			// << std::setw(8) << delta * delta << std::endl;
//
			chi2_per_process += delta * delta;
		}
		// std::cout << "Chi^2/ndof per process: " << chi2_per_process / processes[i].numberOfpoints << " for "  << processes[i].dataCode << std::endl;
		result += chi2_per_process;
	}
	return result;
}


void FitManager::PerformMinimization(const char * outputfile)
{
	if (gMinimizer == 0)
		SetupMinimizer();

	double arglist[10];
	int ierflag = 0;
	arglist[0] = 150;
	arglist[1] = 1.;
	gMinimizer->mnexcm("SHO FCN", arglist, 2, ierflag);
	arglist[1] = 0.;
	gMinimizer->mnexcm("SET STR", arglist + 1,  1, ierflag);
	// arglist[1] = 3.;
	// gMinimizer->mnexcm("SET PRI", arglist + 1,  1, ierflag);

	ierflag = 0;
	arglist[0] = 5000;
	arglist[1] = 0.00001;
	// gMinimizer->mnexcm("MIGRAD", arglist, 2, ierflag);
	gMinimizer->mnexcm("SIMPLEX", arglist, 2, ierflag);
	arglist[0] = 1000;
	// gMinimizer->mnexcm("MIGRAD", arglist, 2, ierflag);

	std::cout << "Showing fcn" << std::endl;
	gMinimizer->mnexcm("SHO FCN", arglist, 2, ierflag);


	// bitp cluster configuration
	// std::fstream fout("/home/okovalen/nonlinear-trajectories/parameters.in");

	// lxplus cluster configuration
	std::fstream fout(outputfile);

	// std::fstream fout("parameters.in");
	for (int i = 0; i < fit_parameters.size(); ++i)
	{
		fit_parameters[i].value	 = 0;
		gMinimizer->GetParameter(i, fit_parameters[i].value, arglist[9]);
		fout << std::setprecision(12) << fit_parameters[i];
	}
}

