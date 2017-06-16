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


	double chi2_per_phys_process = 0;
	for (int i = 0; i < fProcesses.size() ; ++i)
	{
		fModel.SetProcessType(fProcesses[i].dataCode);
		// std::cout << "Processing " << fProcesses[i].dataCode << std::endl;
		chi2_per_phys_process = 0;
		TheoreticalModel computor(fModel);

		int npoints = fProcesses[i].numberOfpoints;
		// Initialize the MPI environment

		int nchunks = npoints / fPoolSize;
		int nreminder = npoints % fPoolSize;
		int current_chunk = (fProcID < nreminder) ? nchunks + 1 : nchunks;
		int start = (fProcID >= nreminder) ? (nchunks + 1) * nreminder + (fProcID - nreminder) * nchunks : current_chunk * fProcID;

		double local_chi2_per_phys_process = 0;
		// #pragma omp parallel for firstprivate(computor) reduction(+:chi2_per_phys_process)
		for (int j = start; j < start + current_chunk; ++j)
		{

			const DataPoint & p = fProcesses[i].experimentalPoints[j];
			// if(p.ignore) continue;

			// std::cout << " Calculating >> " << std::endl;
			double delta = ValueInPoint(p);


			// std::cout << std::setw(8) << p.energy << "\t"
			//           << std::setw(8) << p.t << "\t"
			//           << std::setw(8) << p.observable << "\t"
			//           << std::setw(8) << y << "\terror\t"
			//           << std::setw(8) << p.error << "\t"
			//           << fProcesses[i].dataCode << "\t"
			//           << std::setw(8) << delta * delta
			//           << " fProcID " <<fProcID
			//           <<  std::endl;


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

double FitManagerMPI::ValueInPoint(const DataPoint & p)
{
	if (fTest)
		return 1.;

	double y =  fModel.GetTheoreticalValue(p.energy, p.t);
	double delta =  (p.observable - y) / p.error ;
	return delta;
}
