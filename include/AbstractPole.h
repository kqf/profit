/* 
 * File:   AbstractPole.h
 * Author: sha
 *
 * Created on August 25, 2014, 9:05 AM
 */

#ifndef ABSTRACTPOLE_H
#define	ABSTRACTPOLE_H

#include <complex>

class AbstractPole {
private:
    typedef std::complex<double > complexd; 

public:
    AbstractPole(const bool & odd):
    isOdd(odd)
    {
	complexd i(0, 1); 
	coef = odd ? -i : -1;
    }

    complexd Amplitude(const double & s, const double & t, bool) const; 
    
private:
    virtual complexd PureAmplitude(const double & s, const double & t) const  = 0; 

    bool isOdd; 
    complexd coef; 

};

#endif	/* ABSTRACTPOLE_H */

