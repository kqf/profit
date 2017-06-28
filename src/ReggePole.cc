/*
 * File:   ReggePole.cc
 * Author: sha
 *
 * Created on May 19, 2014, 8:01 PM
 */

#include "ReggePole.h"

#include <iostream>



ReggePole::complexd ReggePole::PureAmplitude(const double & s, const double & t) const
{
    complexd i(0, 1);
    return  g * pow(-1. * i * s, a0 - ap * t) * exp(-B * t);
}

// This funciton creates needed number of poles:
// n -- control number of poles in a model
std::vector<AbstractPole * > ReggePole::MakePoles(const double * p, const int & n)
{
    assert((n % kInputParameters == 0) && "Trying to pass wrong amount of parameters");

    // TODO: Use shared pointers
    std::vector<AbstractPole  * > poles;
    for (int i = 0; i < n; i += kInputParameters)
        poles.push_back( (new ReggePole(p[i], p[i + 1], p[i + 2], p[i + 3], p[i + 4] < 0)) );

    return poles;
}

void ReggePole::SetParameters(const double * pars, int & offset)
{
   a0 = pars[offset];
   ap = pars[offset + 1];
   g = pars[offset + 2];
   B = pars[offset + 3];

   // Don't change the internal property
   // odd = pars[offset + 4] < 0;

   // Update the offset value
   offset += kInputParameters;
}




