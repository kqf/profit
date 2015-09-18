/* 
 * File:   QuadraticPole.cc
 * Author: sha
 * 
 * Created on May 19, 2014, 8:01 PM
 */

#include "QuadraticPole.h"

#include <iostream>



QuadraticPole::complexd QuadraticPole::PureAmplitude(const double & s, const double & t) const
{
    complexd i(0, 1);
    return  g * pow(-1. * i * s, a0 - ap * t - app * t * t) * exp(-B * t);
}

// This funciton creates needed number of poles:
// n -- control number of poles in a model
std::vector<AbstractPole * > QuadraticPole::MakePoles(const double * p, const int & n)
{
    assert((n % nImputParamets == 0) && "Trying to pass wrong amount of parameters"); 

    std::vector<AbstractPole  * > poles; 
    for(int i = 0; i < n; i += nImputParamets)
	poles.push_back( (new QuadraticPole(p[i], p[i + 1], p[i + 2], p[i + 3], p[i + 4], p[i + 5] < 0)) ); 

    return poles; 
}



