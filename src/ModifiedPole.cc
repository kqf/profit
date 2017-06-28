/*
 * File:   ModifiedPole.cc
 * Author: sha
 *
 * Created on August 25, 2014, 10:46 AM
 */

#include "ModifiedPole.h"

AbstractPole::complexd ModifiedPole::PureAmplitude(const double& s, const double& t) const
{
    complexd i(0, 1);
    double alpha = a0 + gamma * ( sqrt(t0) - sqrt(t0 + t) );
    double phi = 1. / pow(1. + t / tau, nu);
    return g * pow(-i * s, alpha - 1) * phi;
}


void ModifiedPole::SetParameters(const double * pars, int & offset)
{
    g     = pars[offset];
    a0    = pars[offset + 1];
    gamma = pars[offset + 2];
    t0    = pars[offset + 3];
    tau   = pars[offset + 4];
    nu    = pars[offset + 5];
    // Don't change the internal properties
    // odd = ...

    // Update the offset value
    offset += kInputParameters;
}

