#ifndef THEORETICALMODEL_H
#define THEORETICALMODEL_H

#include <complex>
#include <cassert>

#include <gsl/gsl_sf_bessel.h>

#include "ReggePole.h"
#include "BesselTransform.h"

// Transformation and it's inverse
//
double g(double t, void * params);
double f (double b, void * params);

class TheoreticalModel
{
private:
    typedef std::complex<double > complexd;

public:
    TheoreticalModel(): fNpars(0) {} ;
    TheoreticalModel(const double * , int);


    void SetParameters(const double *);
    void SetProcessType(const int & t)
    {
        fProcessId = t;
        fOddProcess = fProcessId % 10;
    }

    double GetTheoreticalValue(double  x, double  y) const;
    complexd GetA(double s, double t, bool imag) const;
    double GetH(double b, double s, double t, bool imag = true) const;
    double Geta(double b, double s, double t, bool imag = true) const;
    complexd Unitarize(const complexd &) const;

    double DrawFunction(double * x, double * par);
    int fNpars;
private:
    void PrintFailure(const int &);
    AbstractPole::Poles fPoles;
    complexd fIlambda;   // i times lambda

    int fProcessId;
    bool fOddProcess;
    bool fSkipEvaluation; // flag as true in PrintFailure(), skip as false in SetParameters
};

#endif  /* THEORETICALMODEL_H */

