#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Chi2TestMPI

#include <boost/test/unit_test.hpp>
#include <iostream>

// -- User headers --
#include "PhysicalProcess.h"
#include "FitManager.h"

#include <mpi.h>

namespace tt = boost::test_tools;

using std::cout;
using std::endl;

struct Analysis
{
    FitManager * tManager;
    double tNominal;
    int tProcId;

    Analysis(): tManager(0), tNominal(4712.3606378067634), tProcId(-123)
    {
        int pool_size;

        MPI_Init(NULL, NULL);

        // Get the number of processes
        MPI_Comm_size(MPI_COMM_WORLD, &pool_size);

        // Get the rank of the process
        MPI_Comm_rank(MPI_COMM_WORLD, &tProcId);

        tManager = &FitManager::GetFitManager(pool_size, tProcId);

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
        tManager->GetData("../Data.root", input_vector);

        // NB: Use local copy of parameters in this directory
        //     global config might (it sohould) change
        tManager->GetParameters("parameters.in");

        BOOST_TEST_MESSAGE("Setup a test pole");
    }

    ~Analysis()
    {
        BOOST_TEST_MESSAGE("Teardown the pole");
        MPI_Finalize();
    }
};

BOOST_FIXTURE_TEST_SUITE(AmplitudeChi2TestMPI, Analysis)

BOOST_AUTO_TEST_CASE(Chi2MPI)
{
    BOOST_TEST_MESSAGE("CURRENT PROC ID ");
    BOOST_TEST_MESSAGE(tProcId);

    
    double total_mpi_time = 0;

    total_mpi_time -= MPI_Wtime();
    double diff = tNominal - tManager->PerformMinimization("devnullparameters.in", 0, 0);
    total_mpi_time += MPI_Wtime();

    cout << "Code worked for " << total_mpi_time << endl;

    if (tProcId != 0)
        return;

    BOOST_TEST(diff == 0.0, tt::tolerance(0.00001));
}


BOOST_AUTO_TEST_SUITE_END()