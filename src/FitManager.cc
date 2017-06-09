/*
 * File:   FitManager.cc
 * Author: sha
 *
 * Created on May 4, 2014, 8:56 AM
 */

#include "FitManager.h"
#include "FitManagerMPI.h"

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

#include <stdlib.h>    // quick_exit

FitManager * FitManager::_fInstance = 0;

FitManager & FitManager::GetFitManager(int psize, int pid)
{
	if (_fInstance == 0)
		_fInstance = (psize == 0) ? new FitManager() : new FitManagerMPI(psize, pid);
	return * _fInstance;
}

void FitManager::GetData(const char * filename, std::vector<PhysicalProcess> input)
{
	// Copy vectors
	fProcesses = input;

	// foreach does not plays well with meber function parameters as refs.
	for (int i = 0; i < fProcesses.size(); ++i)
		fProcesses[i].FillFromFile(filename);
}

void FitManager::GetParameters(const char * filename)
{
	fFitParameters = ModelParameter::GetParameters(filename);

	std::copy(fFitParameters.begin(), fFitParameters.end(), std::ostream_iterator<ModelParameter>(std::cout, ""));

	const int inp_length = fFitParameters.size();
	double values[inp_length];

	for (int i = 0; i < fFitParameters.size(); ++i)
		values[i] = fFitParameters[i].value;

	fModel = TheoreticalModel(values, inp_length);

}

static void fcn(int& npar, double* gradients, double& f, double* p, int flag)
{
	FitManager & manager = FitManager::GetFitManager();
	f = manager.chi2(p);
}

void FitManager::SetupMinimizer()
{
	int npars = fFitParameters.size();

	// assert(npars != 0 && "Cannot initialize minimizer with 0 input parameters");
	gMinimizer = new TMinuit(npars);
	gMinimizer->SetFCN(fcn);

	// TODO: Implement parameters properties
	double arglist[10];
	int ierflg = 0;

	arglist[0] = 1;
	gMinimizer->mnexcm("SET ERR", arglist, 1, ierflg);

	for (int i = 0; i < fFitParameters.size(); ++i)
		gMinimizer->mnparm(i
		                   , fFitParameters[i].name
		                   , fFitParameters[i].value
		                   , fFitParameters[i].step_size
		                   , fFitParameters[i].lower_bound
		                   , fFitParameters[i].upper_bound
		                   , ierflg);
}

double FitManager::chi2(const double * parameters)
{
	//static int i = 0;
	//++i;
	//std::cout << "Calling fcn for "  << i << " times" << std::endl;
	if (parameters != 0)
		fModel.SetParameters(parameters);


	double result = 0;
	double chi2_per_process = 0;
	for (int i = 0; i < fProcesses.size() ; ++i )
	{
		fModel.SetProcessType(fProcesses[i].dataCode);
		// std::cout << "Processing " << fProcesses[i].dataCode << std::endl;
		chi2_per_process = 0;
		TheoreticalModel computor(fModel);

		int npoints = fProcesses[i].numberOfpoints;
		#pragma omp parallel for firstprivate(computor) reduction(+:chi2_per_process)
		for (int j = 0;  j <  npoints; ++j)
		{
			const DataPoint & p = fProcesses[i].experimentalPoints[j];
			// TODO: Check the dataset. Should we ignore these values indeed.
			// if(p.ignore) continue;

			// std::cout << " Calculating >> " << std::endl;
			double y =  computor.GetTheoreticalValue(p.energy, p.t);
			double delta =  (p.observable - y) / p.error ;


			// std::cout << std::setw(8) << p.energy << "\t"
			//              << std::setw(8) << p.t << "\t"
			//              << std::setw(8) << p.observable << "\t"
			//              << std::setw(8) << y << "\terror\t"
			//              << std::setw(8) << p.error << "\t"
			// << fProcesses[i].dataCode << "\t"
			// << std::setw(8) << delta * delta << std::endl;
			//

			chi2_per_process += delta * delta;
		}
		std::cout << "Chi^2/ndof per process: " << chi2_per_process / fProcesses[i].numberOfpoints << " for "  << fProcesses[i].dataCode << std::endl;
		result += chi2_per_process;
	}
	return result;
}


double FitManager::GetChi2()
{
	double pars[fFitParameters.size()];
	for (int i = 0; i < fFitParameters.size(); ++i)
		pars[i] = fFitParameters[i].value;

	return chi2(pars);
}


double FitManager::PerformMinimization(const char * outputfile, int nsimplex, int nmigrad)
{
	if (gMinimizer == 0)
		SetupMinimizer();


	if (!nsimplex && !nmigrad)
		return GetChi2();

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
	arglist[0] = nsimplex;
	arglist[1] = 0.00001;
	// gMinimizer->mnexcm("MIGRAD", arglist, 2, ierflag);
	gMinimizer->mnexcm("SIMPLEX", arglist, 2, ierflag);
	arglist[0] = nmigrad;
	gMinimizer->mnexcm("MIGRAD", arglist, 2, ierflag);

	std::cout << "Showing fcn" << std::endl;
	gMinimizer->mnexcm("SHO FCN", arglist, 2, ierflag);


	// bitp cluster configuration
	// std::fstream fout("/home/okovalen/nonlinear-trajectories/parameters.in");

	// lxplus cluster configuration
	std::fstream fout(outputfile);

	// std::fstream fout("parameters.in");
	for (int i = 0; i < fFitParameters.size(); ++i)
	{
		fFitParameters[i].value	 = 0;
		gMinimizer->GetParameter(i, fFitParameters[i].value, arglist[9]);
		fout << std::setw(14) <<  std::setprecision(12) << fFitParameters[i];
	}

	// Return the updated version of chi2 value
	double chi2val = GetChi2();
	return chi2val;
}
