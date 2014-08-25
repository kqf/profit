/* 
 * File:   ModifiedPole.h
 * Author: sha
 *
 * Created on August 25, 2014, 10:46 AM
 */

#ifndef MODIFIEDPOLE_H
#define	MODIFIEDPOLE_H

#include "AbstractPole.h"


class ModifiedPole: public AbstractPole {
public:
    ModifiedPole(const double & a, const double & b,
                 const double & c, const double & d,
	         const double & e, const double & f, 
	         const bool & odd):
	         g(a), a0(b), gamma(c), t0(d), tau(e), nu(f), AbstractPole(odd) {}
private:
    // this is number of input parameters in constructor!
    // if you modify the constructor --- you should modify 
    // this number too !!


    virtual complexd PureAmplitude(const double & s, const double & t) const; 

    double g;
    double a0;
    double gamma;
    double t0; 
    double tau; 
    double nu; 

};

#endif	/* MODIFIEDPOLE_H */

