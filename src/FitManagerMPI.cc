#include "FitManagerMPI.h"

#include <iostream>
#include <iomanip>

#include <omp.h>


#ifdef MPI_VERSION
#include <mpi.h>
#endif

FitManagerMPI::FitManagerMPI(int psize = 1, int pid = 1, bool test = false):
	FitManager(),
	fPoolSize(psize),
	fProcID(pid),
	fTest(test)

{
}


// TODO: Add separate function call for this
//static int i = 0;
//++i;
//std::cout << "Calling fcn for "  << i << " times" << std::endl;


double FitManagerMPI::chi2( double * parameters)
{
	double result = 0;

	// NB: Don't do anything here if MPI is not defined
	//

#ifdef MPI_VERSION
	if (parameters != 0)
	{

		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Bcast(parameters, fModel.npars, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);

		fModel.SetParameters(parameters);
	}


	double chi2_per_phys_process;
	for (int i = 0; i < fProcesses.size() ; ++i)
	{
		chi2_per_phys_process = 0;
		fModel.SetProcessType(fProcesses[i].dataCode);
		TheoreticalModel computor(fModel);

		// Calculate start and stop for each data chunk
		int start, stop;
		IndecesForProcess(fProcesses[i], start, stop);

		double local_chi2_per_phys_process = 0;
		#pragma omp parallel for firstprivate(computor) reduction(+:chi2_per_phys_process)
		for (int j = start; j < stop; ++j)
		{

			const DataPoint & p = fProcesses[i].experimentalPoints[j];
			// if(p.ignore) continue;

			double delta = ValueInPoint(computor, p);

			local_chi2_per_phys_process += delta * delta;
		}
		MPI_Allreduce(&local_chi2_per_phys_process, &chi2_per_phys_process, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

		if (fProcID == 0)
			std::cout << "Chi^2/ndof per process: " << chi2_per_phys_process / fProcesses[i].numberOfpoints << " for "  << fProcesses[i].dataCode << " fProcID " << fProcID <<  std::endl;

		if (fTest)
			chi2_per_phys_process /= fProcesses[i].numberOfpoints;

		result += chi2_per_phys_process;
	}

	MPI_Barrier(MPI_COMM_WORLD);
#endif

	return result;
}

double FitManagerMPI::ValueInPoint(TheoreticalModel & model, const DataPoint & p)
{
	if (fTest)
		return 1.;

	double y =  model.GetTheoreticalValue(p.energy, p.t);
	double delta =  (p.observable - y) / p.error ;
	return delta;

}

void FitManagerMPI::IndecesForProcess(const PhysicalProcess & proc, int & start, int & stop) const
{
	int npoints = proc.numberOfpoints;

	int nchunks = npoints / fPoolSize;
	int nreminder = npoints % fPoolSize;
	int current_chunk = (fProcID < nreminder) ? nchunks + 1 : nchunks;
	start = (fProcID >= nreminder) ? (nchunks + 1) * nreminder + (fProcID - nreminder) * nchunks : current_chunk * fProcID;
	stop = start + current_chunk;
}
