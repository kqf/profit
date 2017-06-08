/*
 * File:   FitManager.h
 * Author: sha
 *
 * Created on May 4, 2014, 8:56 AM
 */

#ifndef FITMANAGER_H
#define FITMANAGER_H

#include "PhysicalProcess.h"
#include "TheoreticalModel.h"
#include "ModelParameter.h"

#include <vector>
#include <cassert>

#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TMinuit.h>


class FitManager
{

public:
    typedef std::vector<PhysicalProcess> DataSet;
    virtual ~FitManager() {}

    static FitManager & GetFitManager(int psize = 0, int pid = 0);
    // TODO: Simplify Further this class
    void GetData(const char * filename, std::vector<PhysicalProcess> input);
    void GetParameters(const char * filename);
    double PerformMinimization(const char * ofile, int nsimplex = 5000, int nmigrad = 1000);
    virtual double chi2(const double * parameters);
    double chi2();


    const DataSet & Data() const
    {
        return processes;
    }

    // Don't make this const as theoretical model can be modified
    // TODO: Correct this behavior
    TheoreticalModel * GetModel()
    {
        return &currentModel;
    }

    void GetParameters(float * pars, int & npars)
    {
        npars = fit_parameters.size();
        pars = new float[npars];
        for (int i = 0; i < npars; ++i)
            pars[i] = fit_parameters[i].value;
    }

protected:
    FitManager(): gMinimizer(0) {}
    FitManager(const FitManager& orig) {}
    FitManager & operator=(const FitManager &) {}

    void FillProcess(PhysicalProcess & proc, const char * filename);
    void SetupMinimizer();
    static FitManager * _instance;
    static bool Cut(const DataPoint & p, int procType);
    static int ConvertEnergy(const double & en) { return int(1000 * en); }

    std::vector<PhysicalProcess> processes;
    std::vector<ModelParameter>  fit_parameters;

    TheoreticalModel currentModel;
    #pragma omp private(currentModel)

    #pragma omp private(processes)
    TMinuit * gMinimizer;

};
#endif  /* FITMANAGER_H */

