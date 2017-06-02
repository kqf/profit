#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Chi2Test

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
    FitManager & manager;
    double nominal;
    int procid;

    Analysis(): manager(FitManager::GetFitManager()), nominal(4712.3606378067634), procid(-123)
    {
        int pool_size;

        MPI_Init(NULL, NULL);

        // Get the number of processes
        MPI_Comm_size(MPI_COMM_WORLD, &pool_size);

        // Get the rank of the process
        MPI_Comm_rank(MPI_COMM_WORLD, &procid);

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
        manager.GetData("../Data.root", input_vector);

        // NB: Use local copy of parameters in this directory
        //     global config might (it sohould) change
        manager.GetParameters("parameters.in");

        BOOST_TEST_MESSAGE("Setup a test pole");
    }

    ~Analysis()
    {
        BOOST_TEST_MESSAGE("Teardown the pole");
        MPI_Finalize();
    }
};

BOOST_FIXTURE_TEST_SUITE(AmplitudeChi2Test, Analysis)

BOOST_AUTO_TEST_CASE(Chi2)
{
    BOOST_TEST_MESSAGE("CURRENT PROC ID ");
    BOOST_TEST_MESSAGE(procid);

    
    double diff = nominal - manager.PerformMinimization("devnullparameters.in", 0, 0);
    if (procid != 0)
        return;


    BOOST_TEST(diff == 0.0, tt::tolerance(0.00001));
}


BOOST_AUTO_TEST_SUITE_END()
