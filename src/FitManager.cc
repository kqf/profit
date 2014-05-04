/* 
 * File:   FitManager.cc
 * Author: sha
 * 
 * Created on May 4, 2014, 8:56 AM
 */

#include "FitManager.h"

#include <iostream>
#include <algorithm>
#include <functional>
#include <TChain.h>

//using std::bind1st;
//using std::mem_fun; 

FitManager::FitManager():
graphs(0),
main_canvas(0)
{ }
//FitManager::FitManager(const FitManager& orig) { }
//FitManager::~FitManager() { }

void FitManager::GetData(const char * filename, std::vector<PhysicalProcess> input)
{
    processes = input; 
    root_file = filename; 

    // foreach does not plays well with meber function parameters as refs.
    for(int i = 0; i < processes.size(); ++i)
	FillProcess(processes[i]);
}

void FitManager::FillProcess(PhysicalProcess & proc)
{
    TChain * dat = new TChain(proc.name);
    dat->Add(root_file);
    float a, b ,c, d;

    dat ->SetBranchAddress("intensity",&a);
    dat ->SetBranchAddress("error",&b);
    dat ->SetBranchAddress("energy",&c);
    dat ->SetBranchAddress("t",&d);
    for (int i = 0 ; i < proc.numberOfpoints; ++i )
    {
	dat->GetEntry(i);
	proc.experimentalPoints.push_back(DataPoint((double)a,(double)b,(double)c,(double)d)); 
    }

    delete dat;
    dat = 0; 
} 

void FitManager::GetParameters()
{
    // TODO: write nice input handler
    double tmp[] = {40.3043, 1.10517, 0.35, 2.32537, 117.221, 0.791348, 1.31638, 1.98679, 102.76, 0.5, 1.2, 8.80651}; 
    int tmp_length = sizeof(tmp) / sizeof(double); 
    fit_parameters = std::vector<double>(tmp, tmp + tmp_length);
    return; 
}

void FitManager::DrawApproximation()
{
//    if(main_canvas)
    main_canvas = new TCanvas("Main Canvas", "experimental data", 800, 600); 
    main_canvas->Divide(3, 2); 
    for(int i = 0; i < processes.size(); ++i)
	CreateGraph(processes[i]);


    for(int i = 0; i < graphs.size(); ++i)
    {
	main_canvas->cd(i + 1);
	if(processes[i].dataCode > 300)
	    gPad->SetLogy(); 
	graphs[i]->Draw("APL");
    }
    main_canvas->Show(); 
}

void FitManager::CreateGraph(PhysicalProcess& proc)
{
    TGraphErrors * graph = new TGraphErrors(); 
    graph->SetName(proc.name);
    graph->SetTitle(proc.title); 
	    
//    TODO: Check wheather numberOfpoints is needed
    for(int i = 0; i < proc.numberOfpoints; ++i)
    {
	// here draw cuts should be applied
	DataPoint point = proc.experimentalPoints[i]; 
	double x = (proc.dataCode  > 300) ? point.t : point.energy;
	double y = point.observable; 
	double dy = point.error; 

	graph ->SetPoint(i, x, y); 
	graph ->SetPointError(i, 0, dy); 
    }
    graphs.push_back(graph); 
}

