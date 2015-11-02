/* 
 * File:   NonlinearPoleT2V3.h
 * Author: sha
 *
 * Created on May 19, 2014, 8:01 PM
 */

#ifndef NONLINEARPOLET2V3_H
#define	NONLINEARPOLET2V3_H

#include <iostream>
#include <iomanip>

#include <complex>
#include <vector>
#include <cassert>

#include "AbstractPole.h"

class NonlinearPoleT2V3 : public AbstractPole {
private:
    typedef std::complex<double > complexd; 

public:
//    complexd Amplitude(const double & s, const double & t, bool) const; 
    static std::vector<AbstractPole *> MakePoles(const double *, const int &); 
    static AbstractPole * MakeNonlinearPole(const double * p, const double & nu, const double & eta);

    virtual void PrintParameters() const {          std::cout << "a0   " << std::setw(8) << a0
                                                              << "\tgamma" << std::setw(8) << gamma
                                                              << "\ttr   " << std::setw(8) << tr
                                                              << "\tg    " << std::setw(8) << g
                                                              << "\tB    " << std::setw(8) << B
                                                              << "\tC    " << std::setw(8) << C
                                                              << "\tnu   " << std::setw(8) << nu
                                                              << "\teta   " << std::setw(8) << eta << std::endl;
                                         }

// private:
    // this is number of input parameters in constructor!
    // if you modify the constructor --- you should modify 
    // this number too !!
    enum {nImputParamets = 9}; 

public:
    NonlinearPoleT2V3(const double & a, const double & b,
                      const double & c, const double & d,
                      const double & e, const double & f,
                      const double & j, const double & k, 
                      bool odd):

    a0(a), gamma(b), tr(c), g(d), B(e), C(f), nu(j), eta(k), AbstractPole(odd) {}

private:
    
    virtual complexd PureAmplitude(const double & s, const double & t) const ; 

    double a0;
    double gamma;
    double tr;
    double g;
    double B; 
    double C; 
    double nu;
    double eta;
};

#endif	/* NONLINEARPOLET2V3_H */

