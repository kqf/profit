/*
 * File:   AbstractPole.h
 * Author: sha
 *
 * Created on August 25, 2014, 9:05 AM
 */

#ifndef ABSTRACTPOLE_H
#define ABSTRACTPOLE_H

#include <complex>
#include <vector>
#include <memory>


class AbstractPole
{
protected:
    typedef std::complex<double > complexd;

public:
    typedef std::vector<std::shared_ptr<AbstractPole> > Poles;
    AbstractPole(const bool & odd):
        isOdd(odd)
    {
        complexd i(0, 1);
        coef = odd ? -i : -1;
    }

    complexd Amplitude(const double & s, const double & t, bool) const;
    virtual void PrintParameters() const = 0;
    virtual void SetParameters(const double * pars, int & offset) = 0;
    static const int kInputParameters = 0;

private:
    virtual complexd PureAmplitude(const double & s, const double & t) const  = 0;

protected:
    bool isOdd;

private:
    complexd coef;

};

#endif  /* ABSTRACTPOLE_H */

