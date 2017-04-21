#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Chi2Test

#include <boost/test/unit_test.hpp>
#include <iostream>

// -- User headers --
#include "PhysicalProcess.h"
#include "FitManager.h"



namespace tt = boost::test_tools;

using std::cout;
using std::endl;

struct Analysis
{
    FitManager & manager;
    double nominal;

    Analysis(): manager(FitManager::GetFitManager()), nominal(137.0)
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

        // NB: Use local copy of parameters in this directory
        //     global config might (it sohould) change
        manager.GetParameters("parameters.in");

        BOOST_TEST_MESSAGE("Setup a test pole");
    }

    ~Analysis()
    {
        BOOST_TEST_MESSAGE("Teardown the pole");
    }
};

BOOST_FIXTURE_TEST_SUITE(AmplitudeChi2Test, Analysis)

BOOST_AUTO_TEST_CASE(Chi2)
{
    double diff = nominal - manager.PerformMinimization("devnullparameters.in", 0);
    BOOST_TEST(diff == 0.0, tt::tolerance(0.00001));
}


BOOST_AUTO_TEST_SUITE_END()