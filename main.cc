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


using namespace std;


int main(int argc, char** argv)
{


    TApplication * app = new TApplication("ProFit", &argc, argv);

    PhysicalProcess input_array[] = { PhysicalProcess("sigma_pp",    110, "#sigma_{pp}", 266),
                                      PhysicalProcess("sigma_pbarp", 111, "#sigma_{p#bar{p}}", 444),
				      PhysicalProcess("rho_pp", 210, "#rho_{pp}", 116),
                                      PhysicalProcess("rho_pbarp", 211, "#rho_{p#bar{p}}", 90),
                                      PhysicalProcess("ds_pp", 310, "#frac{d#sigma}{dt}_{pp}", 1633),
                                      PhysicalProcess("ds_pbarp", 311, "#frac{d#sigma}{dt}_{p#bar{p}}", 1249) };

    std::vector<PhysicalProcess> input_vector(input_array, input_array + sizeof(input_array) / sizeof(PhysicalProcess));

    FitManager & manager =   FitManager::GetFitManager();
    manager.GetData("Data.root", input_vector); 
    manager.GetParameters(); 
    manager.DrawApproximation(); 

//    TStopwatch timer; 
//    timer.Start(); 
//    std::cout << "Here is unminimized chi2 " << manager.chi2(0) << std::endl;
//    timer.Stop(); 
//    std::cout << "It takes " << timer.RealTime() / 60 << " to calculate chi^2." << std::endl; 


    app->Run();
    return 0;
}

