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

#include <mpi.h>

FitManagerMPI::FitManagerMPI(int psize = 1, int pid = 1):
	FitManager(),
	pool_size(psize),
	procid(pid)

{
}

double FitManagerMPI::chi2(const double * parameters)
{
	//static int i = 0;
	//++i;
	//std::cout << "Calling fcn for "  << i << " times" << std::endl;
	if (parameters != 0)
		fModel.SetParameters(parameters);


	double result = 0;
	double chi2_per_phys_process = 0;
	for (int i = 0; i < fProcesses.size() ; ++i)
	{
		fModel.SetProcessType(fProcesses[i].dataCode);
		// std::cout << "Processing " << fProcesses[i].dataCode << std::endl;
		chi2_per_phys_process = 0;
		TheoreticalModel computor(fModel);

		int npoints = fProcesses[i].numberOfpoints;
		// Initialize the MPI environment

		int nchunks = npoints / pool_size;
		int nreminder = npoints % pool_size;
		int current_chunk = (procid < nreminder) ? nchunks + 1 : nchunks;
		int start = (procid >= nreminder) ? (nchunks + 1) * nreminder + (procid - nreminder) * nchunks : current_chunk * procid;

		double local_chi2_per_phys_process = 0;
		// #pragma omp parallel for firstprivate(computor) reduction(+:chi2_per_phys_process)
		for (int j = start; j < start + current_chunk; ++j)
		{

			const DataPoint & p = fProcesses[i].experimentalPoints[j];
			// if(p.ignore) continue;

			// std::cout << " Calculating >> " << std::endl;
			double y =  computor.GetTheoreticalValue(p.energy, p.t);
			double delta =  (p.observable - y) / p.error ;


			// std::cout << std::setw(8) << p.energy << "\t"
			//           << std::setw(8) << p.t << "\t"
			//           << std::setw(8) << p.observable << "\t"
			//           << std::setw(8) << y << "\terror\t"
			//           << std::setw(8) << p.error << "\t"
			//           << fProcesses[i].dataCode << "\t"
			//           << std::setw(8) << delta * delta
			//           << " procid " << procid
			//           <<  std::endl;


			local_chi2_per_phys_process += delta * delta;
		}
		MPI_Reduce(&local_chi2_per_phys_process, &chi2_per_phys_process, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		if(procid == 0)
			std::cout << "Chi^2/ndof per process: " << chi2_per_phys_process / fProcesses[i].numberOfpoints << " for "  << fProcesses[i].dataCode << " procid " << procid <<  std::endl;

		result += chi2_per_phys_process;
	}
	return result;
}