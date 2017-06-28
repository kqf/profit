/*
 * File:   NonlinearPoleT2V3.cc
 * Author: sha
 *
 * Created on May 19, 2014, 8:01 PM
 */

#include "NonlinearPoleT2V3.h"

#include <iostream>



NonlinearPoleT2V3::complexd NonlinearPoleT2V3::PureAmplitude(const double & s, const double & t) const
{

    double tt = -t;
    double alpha = isOdd ? a0 + gamma * tt : a0 + gamma * ( pow(tr, nu) -  pow(tr - tt, nu) );

    // std::cout << "alpha " << nu  << " "  << eta << std::endl;

    // double V = exp( B * (sqrt(Tau) - sqrt(Tau - tt)) );// beta -> B
    // double V = exp( B * (pow(Tau, nu) - pow(Tau - tt, nu)) )// beta -> B
    // PrintParameters();

    double V =  C * exp(B * tt) + (1. - C)  * pow(1. - tt / Tau, -eta);
    complexd i(0, 1);
    return  g * pow(-1. * i * s, alpha) * V;
}

// This funciton creates needed number of poles:
// n -- control number of poles in a model
std::vector<AbstractPole * > NonlinearPoleT2V3::MakePoles(const double * p, const int & n)
{
    assert((n % kInputParameters == 0) && "Trying to pass wrong amount of parameters");

    std::vector<AbstractPole  * > poles;
    for (int i = 0; i < n; i += kInputParameters)
        poles.push_back( (new NonlinearPoleT2V3(p[i], p[i + 1], p[i + 2], p[i + 3],
                                                p[i + 4], p[i + 5], p[i + 6], p[i + 7], p[i + 8], p[i + 9] < 0 ) ) );

    return poles;
}


void NonlinearPoleT2V3::SetParameters(const double * pars, int & offset)
{
    a0     = pars[offset];
    gamma  = pars[offset + 1];
    tr     = pars[offset + 2];
    g      = pars[offset + 3];
    B      = pars[offset + 4];
    Tau    = pars[offset + 5];
    C      = pars[offset + 6];
    eta    = pars[offset + 7];
    nu     = (!isOdd) ? 0 : pars[0]; // No offset here

    // Don't change the internal properties
    // odd = ...

    // Update the offset value
    offset += kInputParameters  - 1;
}

