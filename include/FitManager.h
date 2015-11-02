/* 
 * File:   FitManager.h
 * Author: sha
 *
 * Created on May 4, 2014, 8:56 AM
 */

#ifndef FITMANAGER_H
#define	FITMANAGER_H

#include "PhysicalProcess.h"
#include "TheoreticalModel.h"
#include "ModelParameter.h"

#include <vector>
#include <cassert>

#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TMinuit.h>


class FitManager {

public:
//    FitManager(const FitManager& orig);
//    virtual ~FitManager();

    static FitManager & GetFitManager(); 
    void GetData(const char * filename, std::vector<PhysicalProcess> input);
    void GetParameters(const char * filename);
    void DrawApproximation();
    void PerformMinimization();
    double chi2(const double * parameters) ; 

private:
    FitManager(); 
    FitManager(const FitManager& orig) {}
    FitManager & operator=(const FitManager &) {}

    void FillProcess(PhysicalProcess & proc); 
    void CreateGraph(PhysicalProcess & proc); 
    void DrawFitFunction(PhysicalProcess & proc); 
    void SetupMinimizer();
    static FitManager * _instance; 
    static bool Cut(const DataPoint & p, int procType); 
    // Needed to energy comparison:
    static int ConvertEnergy(const double & en) { return int(1000 * en); }

//    TODO: rearrange data fields.

    std::vector<ModelParameter>     fit_parameters; 
    std::vector<PhysicalProcess> processes; 
//#pragma omp private(processes)
    std::vector<TGraphErrors *>     graphs; 
    TCanvas * main_canvas; 
    TMinuit * gMinimizer; 

    const char * root_file; 
    TheoreticalModel currentModel; 
#pragma omp private(currentModel)
    TF1 * fitFunction; 
    double ds_pp_energy;
    double ds_pbp_energy;
};

#endif	/* FITMANAGER_H */

