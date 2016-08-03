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
    static AbstractPole * MakeNonlinearPole(const double * p, const double & nu);

    virtual void PrintParameters() const {          std::cout << "a0   " << std::setw(8) << a0
                                                              << "\tgamma" << std::setw(8) << gamma
                                                              << "\ttr   " << std::setw(8) << tr
                                                              << "\tg    " << std::setw(8) << g
                                                              << "\tB    " << std::setw(8) << B
                                                              << "\tTau  " << std::setw(8) << Tau 
                                                              << "\tC    " << std::setw(8) << C
                                                              << "\teta  " << std::setw(8) << eta 
                                                              << "\tnu   " << std::setw(8) << nu
                                                              << "\tOdd  " << std::setw(8) << isOdd << std::endl;
                                         }

// private:
    // this is number of input parameters in constructor!
    // if you modify the constructor --- you should modify 
    // this number too !!
    enum {nImputParamets = 10}; 

public:
    NonlinearPoleT2V3(const double & a, const double & b,
                      const double & c, const double & d,
                      const double & e, const double & f,
                      const double & j, const double & k, 
                      const double & l, bool odd):

    a0(a), gamma(b), tr(c), g(d), B(e), Tau(f), C(j), eta(k), nu(l), AbstractPole(odd) {/*PrintParameters();*/}

private:
    
    virtual complexd PureAmplitude(const double & s, const double & t) const ; 

    double a0;
    double gamma;
    double tr;
    double g;
    double Tau; 
    double B; 
    double C; 
    double nu;
    double eta;
};

#endif	/* NONLINEARPOLET2V3_H */