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
AbstractPole::Poles QuadraticPole::MakePoles(const double * p, const int & n)
{
    assert((n % kInputParameters == 0) && "Trying to pass wrong amount of parameters");

    AbstractPole::Poles poles;
    for (int i = 0; i < n; i += kInputParameters)
    {
        std::shared_ptr<AbstractPole> pole(new QuadraticPole(p[i], p[i + 1], p[i + 2], p[i + 3], p[i + 4], p[i + 5] < 0));
        poles.push_back(pole);
    }

    return poles;
}



void QuadraticPole::SetParameters(const double * pars, int & offset)
{
    g     = pars[offset];
    a0  = pars[offset + 1];
    ap     = pars[offset + 2];
    B      = pars[offset + 3];
    app      = pars[offset + 4];
    // Don't change the internal properties
    // odd = ...

    // Update the offset value
    offset += kInputParameters;
}

