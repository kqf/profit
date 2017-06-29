/*
 * File:   ReggePole.h
 * Author: sha
 *
 * Created on May 19, 2014, 8:01 PM
 */

#ifndef REGGEPOLE_H
#define REGGEPOLE_H

#include <iostream>
#include <complex>
#include <cassert>

#include "AbstractPole.h"

class ReggePole : public AbstractPole
{
private:
    typedef std::complex<double > complexd;

public:
//    complexd Amplitude(const double & s, const double & t, bool) const;
    static AbstractPole::Poles MakePoles(const double *, const int &, int &);
    virtual void PrintParameters() const
    {
        std::cout << "g  "   << g
                  << " a0 "  << a0
                  << " ap "  << ap
                  << " B "   << B << std::endl;
    }
    virtual void SetParameters(const double * pars, int & offset);
    static const int kInputParameters = 5;

private:
    // this is number of input parameters in constructor!
    // if you modify the constructor --- you should modify
    // this number too !!

    ReggePole(const double & a, const double & b,
              const double & c, const double & d, const bool & odd):
        a0(a), ap(b), g(c),  B(d), AbstractPole(odd) {}

    virtual complexd PureAmplitude(const double & s, const double & t) const ;

    double a0;
    double ap;
    double g;
    double B;
};

#endif  /* REGGEPOLE_H */

