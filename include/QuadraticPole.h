/* 
 * File:   QuadraticPole.h
 * Author: sha
 *
 * Created on May 19, 2014, 8:01 PM
 */

#ifndef QUADRATICPOLE_H
#define	QUADRATICPOLE_H

#include <iostream>
#include <iomanip>

#include <complex>
#include <vector>
#include <cassert>

#include "AbstractPole.h"

class QuadraticPole : public AbstractPole {
private:
    typedef std::complex<double > complexd; 

public:
//    complexd Amplitude(const double & s, const double & t, bool) const; 
    static std::vector<AbstractPole *> MakePoles(const double *, const int &); 

    virtual void PrintParameters() const { std::cout<< "g  "   << std::setw(8) << g 
                                                    << "\ta0 "  << std::setw(8) << a0
                                                    << "\tap "  << std::setw(8) << ap
                                                    << "\tapp " << std::setw(8) << app
                                                    << "\tB "   << std::setw(8) << B << std::endl;
                                         }

    virtual void SetParameters(const double * pars, int & offset);
private:
    // this is number of input parameters in constructor!
    // if you modify the constructor --- you should modify 
    // this number too !!
    static const int kInputParameters = 6;

public:
    QuadraticPole(const double & a, const double & b,
                  const double & c, const double & d,
                  const double & e, const bool & odd):
	      g(a), a0(b), ap(c), B(d), app(e), AbstractPole(odd) {}

private:
    
    virtual complexd PureAmplitude(const double & s, const double & t) const ; 

    double g;
    double a0;
    double ap;
    double B; 
    double app; // second order term
};

#endif	/* QUADRATICPOLE_H */

