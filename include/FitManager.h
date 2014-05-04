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

#include <vector>

#include <TGraphErrors.h>
#include <TCanvas.h>


class FitManager {

public:
    FitManager();
//    FitManager(const FitManager& orig);
//    virtual ~FitManager();

    void GetData(const char * filename, std::vector<PhysicalProcess> input);
    void GetParameters();
    void DrawApproximation();

private:
    void FillProcess(PhysicalProcess & proc); 
    void CreateGraph(PhysicalProcess & proc); 
    void DrawFitFunction(PhysicalProcess & proc); 


    std::vector<double>     fit_parameters; 
    std::vector<PhysicalProcess> processes; 
    std::vector<TGraphErrors *>     graphs; 
    TCanvas * main_canvas; 
    const char * root_file; 
    TheoreticalModel currentModel; 
    TF1 * fitFunction; 
    double ds_pp_energy;
    double ds_pbp_energy;
};

#endif	/* FITMANAGER_H */

