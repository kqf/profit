/* 
 * File:   ReggePole.cc
 * Author: sha
 * 
 * Created on May 19, 2014, 8:01 PM
 */

#include "ReggePole.h"

#include <iostream>


ReggePole::complexd ReggePole::Amplitude(const double & s, const double & t, bool anti) const
{
    complexd i(0, 1);
    double crossing = (anti && isOdd) ? -1 : 1; 
    return coef * g * pow(-1. * i * s, a0 - ap * t) * exp(-B * t) * crossing;
}

// This funciton creates needed number of poles:
// n -- control number of poles in a model
std::vector<ReggePole > ReggePole::MakePoles(const double * p, const int & n)
{
    assert((n % nImputParamets == 0) && "Trying to pass wrong amount of parameters"); 

    std::vector<ReggePole > poles; 
    for(int i = 0; i < n; i += nImputParamets)
	poles.push_back(ReggePole(p[i], p[i + 1], p[i + 2], p[i + 3], p[i + 4] < 0)); 

    return poles; 
}



