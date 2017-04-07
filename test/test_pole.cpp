#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE NonlinearPole
#include <boost/test/unit_test.hpp>
#include <NonlinearPoleT2V3.h>
#include <iostream>

namespace tt = boost::test_tools;

using std::cout;
using std::endl;

struct Pole
{
    NonlinearPoleT2V3 pole;
    double s;
    double t;

    Pole() : pole(1, 1, 1, 1, 1, 1, 1, 1, 1, 1), s(5), t(0.1)
    {
        BOOST_TEST_MESSAGE("Setup a test pole");
    }

    ~Pole()
    {
        BOOST_TEST_MESSAGE("Teardown the pole");
    }
};

BOOST_FIXTURE_TEST_SUITE(NonlinearPoleTest, Pole)

BOOST_AUTO_TEST_CASE(CheckParameters)
{

    pole.PrintParameters();
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(Odd)
{
    std::complex<double> nominal(3.8042, 0.602526);

    auto a = pole.Amplitude(s, t, true);
    auto diff = a - nominal;

    BOOST_TEST(diff.imag() == 0.0, tt::tolerance(0.00001));
    BOOST_TEST(diff.real() == 0.0, tt::tolerance(0.00001));
}

BOOST_AUTO_TEST_CASE(Even)
{
    std::complex<double> nominal(-3.8042, -0.602526);

    auto a = pole.Amplitude(s, t, false);
    auto diff = a - nominal;

    BOOST_TEST(diff.imag() == 0.0, tt::tolerance(0.00001));
    BOOST_TEST(diff.real() == 0.0, tt::tolerance(0.00001));
 }

BOOST_AUTO_TEST_SUITE_END()