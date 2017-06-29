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
AbstractPole::Poles ReggePole::MakePoles(const double * p, const int & n, int & offset)
{
	// TODO: Use shared pointers
	AbstractPole::Poles poles;
	for (int i = 0; i < n; ++i)
	{
		AbstractPole * pole = new ReggePole(p[offset], p[offset + 1], p[offset + 2], p[offset + 3], p[offset + 4] < 0);
		poles.push_back(std::shared_ptr<AbstractPole>(pole));
		offset += kInputParameters;
	}
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

