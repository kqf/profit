/* 
 * File:   TheoreticalModel.h
 * Author: sha
 *
 * Created on May 4, 2014, 3:20 PM
 */

#ifndef THEORETICALMODEL_H
#define	THEORETICALMODEL_H

#include <complex>
#include <vector>
#include <cassert>

#include <gsl/gsl_integration.h>
#include <gsl/gsl_sf_bessel.h>

#include "ReggePole.h"

// Transformation and it's inverse

double g(double x, void * params);
double f (double x, void * params);

class TheoreticalModel {
private:
    typedef std::complex<double > complexd; 
public:
    TheoreticalModel():npars(0) {} ;
//    TheoreticalModel(TheoreticalModel & other);
//    TheoreticalModel & operator=(const TheoreticalModel & other);

    TheoreticalModel(const double * ,int);

    void SetParameters(const double *); 
    void SetProcessType(const int & t)  { processType = t; }

    double GetTheoreticalValue(double  x, double  y); 
    complexd GetA(bool); 
    double GetH(double); 
    double Geta(double); 
    complexd Unitarize(const complexd &); 

    double BesselTransform(double (*function)(double, void *), bool whole_range = true);
    double DrawFunction(double * x, double * par); 
    int npars; 
private:
    void PrintFailure(const int &);
    std::vector<AbstractPole * > poles; 
    complexd ilambda; 
    
    double b, s, t; 
    int processType; 
    bool calculateImagH, calculateImagh; 
    bool skipEvaluation; // flag as true in PrintFailure(), skip as false in SetParameters
                          // this should save a lot of time.
};

#endif	/* THEORETICALMODEL_H */

