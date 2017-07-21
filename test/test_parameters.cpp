#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ParametersTest

#include <boost/test/unit_test.hpp>
#include <iostream>

// -- User headers --
#include "PhysicalProcess.h"
#include "FitManager.h"


// This test is needed to check if the parameters after the minimization
// give the same result without openMPI.
// Also it's important to know if the file was written correctly.
//


namespace tt = boost::test_tools;

using std::cout;
using std::endl;

struct Analysis
{
    FitManager & manager;
    Analysis(): manager(FitManager::GetFitManager())
    {
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

        // NB: This points to the outside of the test directory,
        //     hence to the latest version of analysis parameters
        manager.GetParameters("../parameters.in");

        BOOST_TEST_MESSAGE("Setting up the analys");
    }

    ~Analysis()
    {
        BOOST_TEST_MESSAGE("Teardown the analysis");
    }
};

BOOST_FIXTURE_TEST_SUITE(AmplitudeParametersTest, Analysis)

BOOST_AUTO_TEST_CASE(Parameters)
{
    // Put here some fake name in order to leave the parameters file untouched
    float chi2 = manager.PerformMinimization("devnullparameters.in", 0, 0);
    std::cout << "The chi2 is: " << chi2 << std::endl;
}


BOOST_AUTO_TEST_SUITE_END()
