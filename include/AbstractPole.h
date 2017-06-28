/*
 * File:   AbstractPole.h
 * Author: sha
 *
 * Created on August 25, 2014, 9:05 AM
 */

#ifndef ABSTRACTPOLE_H
#define ABSTRACTPOLE_H

#include <complex>


class AbstractPole
{
protected:
    typedef std::complex<double > complexd;

public:
    AbstractPole(const bool & odd):
        isOdd(odd)
    {
        complexd i(0, 1);
        coef = odd ? -i : -1;
    }

    complexd Amplitude(const double & s, const double & t, bool) const;
    virtual void PrintParameters() const = 0;
    virtual void SetParameters(const double * pars, int & offset) = 0;

private:
    virtual complexd PureAmplitude(const double & s, const double & t) const  = 0;

protected:
    static const int kInputParameters = 0;
    bool isOdd;

private:
    complexd coef;

};

#endif  /* ABSTRACTPOLE_H */

