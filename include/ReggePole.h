/* 
 * File:   ReggePole.h
 * Author: sha
 *
 * Created on May 19, 2014, 8:01 PM
 */

#ifndef REGGEPOLE_H
#define	REGGEPOLE_H

#include <iostream>
#include <complex>
#include <vector>
#include <cassert>

#include "AbstractPole.h"

class ReggePole : public AbstractPole {
private:
    typedef std::complex<double > complexd; 

public:
//    complexd Amplitude(const double & s, const double & t, bool) const; 
    static std::vector<AbstractPole *> MakePoles(const double *, const int &); 
    virtual void PrintParameters() const { std::cout << "g  "   << g 
                                                    << " a0 "  << a0
                                                    << " ap "  << ap
                                                    << " B "   << B << std::endl;
                                         }
private:
    // this is number of input parameters in constructor!
    // if you modify the constructor --- you should modify 
    // this number too !!
    enum {nImputParamets = 5}; 

    ReggePole(const double & a, const double & b,
              const double & c, const double & d, const bool & odd):
	      g(a), a0(b), ap(c), B(d), AbstractPole(odd) {}

    virtual complexd PureAmplitude(const double & s, const double & t) const ; 

    double g;
    double a0;
    double ap;
    double B; 
};

#endif	/* REGGEPOLE_H */

