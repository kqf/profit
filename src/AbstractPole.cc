/* 
 * File:   AbstractPole.cc
 * Author: sha
 * 
 * Created on August 25, 2014, 9:05 AM
 */

#include "AbstractPole.h"

AbstractPole::complexd AbstractPole::Amplitude(const double & s, const double & t, bool anti) const
{
    complexd i(0, 1);
    double crossing = (anti && isOdd) ? -1 : 1; 
    return crossing * coef * PureAmplitude(s, t); 
}


