/*
 * File:   main.cpp
 * Author: sha
 *
 * Created on August 5, 2013, 1:19 PM
 */


#include <iostream>

#include <TString.h>
#include <TApplication.h>
#include <TStopwatch.h>



#include "FitManager.h"
#include "PhysicalProcess.h"


#include <mpi.h>
using namespace std;


int main(int argc, char** argv)
{

	// MPI_Init(NULL, NULL);

	int pool_size = 0;
	int procid = 0;

	// Get the number of processes
	// MPI_Comm_size(MPI_COMM_WORLD, &pool_size);

	// Get the rank of the process
	// MPI_Comm_rank(MPI_COMM_WORLD, &procid);


	TString path = (argc > 1) ? argv[1] : "/afs/cern.ch/user/o/okovalen/private/bitp/regge-amplitude-analysis/";
	std::cout << "Your path is: " << path << endl;

	// TApplication * app = new TApplication("ProFit", &argc, argv);
	// new data
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

	FitManager & manager = FitManager::GetFitManager(pool_size, procid);

	// These are configurations for bitp cluster
	// manager.GetData("/home/okovalen/nonlinear-trajectories/Data.root", input_vector);
	// manager.GetParameters("/home/okovalen/nonlinear-trajectories/parameters.in");

	// These are configurations for lxplus cluster
	manager.GetData(path + "Data.root", input_vector);
	manager.GetParameters(path + "parameters.in");
	// manager.DrawApproximation();


	TStopwatch timer;
	timer.Start();
	manager.PerformMinimization(path + "parameters.in");
	timer.Stop();
	std::cout << "It takes " << timer.RealTime() / 60 << " min to calculate chi^2." << std::endl;


	// MPI_Finalize();
	// app->Run();
	return 0;
}


