/* 
 * File:   main.cpp
 * Author: sha
 *
 * Created on August 5, 2013, 1:19 PM
 */

#include <iostream>
#include <TApplication.h>
#include <TStopwatch.h>



#include "FitManager.h"
#include "PhysicalProcess.h"
#include "ncurses.h"


using namespace std;


int main(int argc, char** argv)
{
	TApplication * app = new TApplication("ProFit", &argc, argv);
	// new data
	PhysicalProcess input_array[] = {	PhysicalProcess("sigma_pp",    110, "#sigma_{pp}"),
										PhysicalProcess("sigma_pbarp", 111, "#sigma_{p#bar{p}}"),
										PhysicalProcess("rho_pp",      210, "#rho_{pp}"),
										PhysicalProcess("rho_pbarp",   211, "#rho_{p#bar{p}}"),
										PhysicalProcess("ds_pp",       310, "#frac{d#sigma}{dt}_{pp}"),
										PhysicalProcess("ds_pbarp",    311, "#frac{d#sigma}{dt}_{p#bar{p}}") };

	std::vector<PhysicalProcess> input_vector(input_array, input_array + sizeof(input_array) / sizeof(PhysicalProcess));

	FitManager & manager = FitManager::GetFitManager();
	manager.GetData("/afs/cern.ch/user/o/okovalen/private/bitp/regge-amplitude-analysis/Data.root", input_vector); 
	// manager.GetData("Data.root", input_vector);
	manager.GetParameters("/afs/cern.ch/user/o/okovalen/private/bitp/regge-amplitude-analysis/parameters.in"); 
	// manager.GetParameters("linear_parameters2.in"); 
	// manager.DrawApproximation(); 

   TStopwatch timer; 
   timer.Start(); 
	 manager.PerformMinimization(); 
   timer.Stop(); 
   std::cout << "It takes " << timer.RealTime() / 60 << " min to calculate chi^2." << std::endl; 


   	beep();
	// app->Run();
	return 0;
}

