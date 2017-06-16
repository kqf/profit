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

#include <TMinuit.h>
#define MPI_VERSION 2


class FitManager
{

public:
    typedef std::vector<PhysicalProcess> DataSet;
    virtual ~FitManager() {}
    static FitManager & GetFitManager(int psize = 0, int pid = 0, bool test = false);

    void GetData(const char * filename, std::vector<PhysicalProcess> input);
    void GetParameters(const char * filename);

    double PerformMinimization(const char * ofile, int nsimplex = 5000, int nmigrad = 1000);
    virtual double chi2(double * parameters);
    double GetChi2();


    const DataSet & Data() const
    {
        return fProcesses;
    }

    // Don't make this const as theoretical model can be modified
    // TODO: Correct this behavior
    TheoreticalModel * Model()
    {
        return &fModel;
    }

    void Parameters(float * pars, int & npars) const
    {
        npars = fFitParameters.size();
        pars = new float[npars];
        for (int i = 0; i < npars; ++i)
            pars[i] = fFitParameters[i].value;
    }

protected:
    FitManager(): gMinimizer(0) {}
    FitManager(const FitManager& orig) {}
    FitManager & operator=(const FitManager &) {}

    void SetupMinimizer();
    static FitManager * _fInstance;

    std::vector<PhysicalProcess> fProcesses;
    std::vector<ModelParameter>  fFitParameters;

    TheoreticalModel fModel;
    #pragma omp private(fModel)

    #pragma omp private(fProcesses)
    TMinuit * gMinimizer;

};
#endif  /* FITMANAGER_H */

