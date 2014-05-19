/* 
 * File:   ReggePole.h
 * Author: sha
 *
 * Created on May 19, 2014, 8:01 PM
 */

#ifndef REGGEPOLE_H
#define	REGGEPOLE_H

#include <complex>
#include <vector>
#include <cassert>

class ReggePole {
private:
    typedef std::complex<double > complexd; 

public:
    complexd Amplitude(const double & s, const double & t) const; 
    static std::vector<ReggePole > MakePoles(const double *, const int &); 

private:
    // this is number of input parameters in constructor!
    // if you modify the constructor --- you should modify 
    // this number too !!
    enum {nImputParamets = 5}; 
    ReggePole(const double & a, const double & b,
              const double & c, const double & d, const bool &odd):
	      g(a), a0(b), ap(c), B(d)
    {
	complexd i(0, 1); 
	coef = odd ? -i : -1; 
    }

    complexd coef; 
    double g;
    double a0;
    double ap;
    double B; 
};

#endif	/* REGGEPOLE_H */

