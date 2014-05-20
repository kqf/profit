/* 
 * File:   TheoreticalModel.cc
 * Author: sha
 * 
 * Created on May 4, 2014, 3:20 PM
 */

#include "TheoreticalModel.h"

#include <iostream>
TheoreticalModel::TheoreticalModel(const double * par, int n):npars(n)
{
    SetParameters(par); 
}

void TheoreticalModel::SetParameters(const double* par)
{
    poles = ReggePole::MakePoles(par, npars - 1); 
    // TODO: check correctness of 2 * i * lambda
    ilambda = complexd(0, 2 * par[npars - 1]); 
}

double TheoreticalModel::GetTheoreticalValue(const double& energy
					    ,const double& transverse_momentum)
{
    assert(npars != 0 && "Parameters are not set");
    s = energy * energy; 
    t = transverse_momentum; 

    double k = 0.3893797; 
    double mp = 0.938270; 

    
    double mp_squared = mp * mp; 
    double flux = sqrt( pow(s - 2 * mp_squared, 2) - 4 * pow(mp_squared, 2) ); 
    double observable = 0; 

    int observableType = int(processType / 100); 
    complexd A = GetA(observableType == 1);  // only 110 and 111 need only image part
    switch(observableType)
    {
	case 1:
	    observable =  (k / flux) * (8 * M_PI * s) * A.imag();
	    break; 
	case 2:
	    observable = A.real() / A.imag(); 
	    break; 
        case 3:
	    // TODO: Check wheather this formula is valid
	    observable = std::norm(A) * k * 4 * M_PI ;//  * (8 * M_PI * s) * k / ( 16 * M_PI * s  )  ; 
	    break;
	default:
	    assert(true && "Unexpected observable type"); 
    }

    return observable; 
}

TheoreticalModel::complexd TheoreticalModel::GetA(bool onlyImage)
{
    calculateImagH = true;
    double im  = BesselTransform(f, false); 
    double re = 0;
    
    if(onlyImage)
	return complexd(0, im);

    calculateImagH = false; // calculate ReH
    re = BesselTransform(f, false);

    return complexd(re, im);
}

double TheoreticalModel::BesselTransform(double(*function)(double,void*), bool whole_range)
{
    double result = 0;
    double error  = 0;
    double precision_abs = 1e-3;
    double precision_rel = 1e-3;
    int workspace = 1e+4;
    
    
    gsl_integration_workspace * w = gsl_integration_workspace_alloc (workspace);
    gsl_function F;

    F.function = function;
    F.params = (void *) this;

    if(whole_range)
	gsl_integration_qagiu (&F, 0, precision_abs, precision_rel, workspace, w, &result, &error); 
    else
        gsl_integration_qags  (&F, 0, 40, precision_abs, precision_rel, workspace, w, &result, &error); 

    gsl_integration_workspace_free (w);
    return result;
}

double TheoreticalModel::GetH(double impactb)
{
    b = impactb; 

    calculateImagh = false;
    double realh = BesselTransform(g, true);

    b = impactb; 

    calculateImagh = true;
    double imagh = BesselTransform(g, true);
    
    complexd h = std::complex<double >(realh, imagh) / (8 * M_PI * s);
    complexd H = Unitarize(h);

    b = impactb; 
    double integrand =  b * gsl_sf_bessel_J0(sqrt(t) * b); 
    if(calculateImagH)
	return H.imag() * integrand;
    return H.real() * integrand ;
}

TheoreticalModel::complexd TheoreticalModel::Unitarize(const complexd& h)
{
    // TODO: Try to write more simple variant
    return h / ( -0.5 * ilambda * h + std::complex<double >(1, 0) );
}

double TheoreticalModel::Geta(double q)
{
    complexd result = 0;
    
    for(int i = 0; i < poles.size(); ++i)
	result += poles[i].Amplitude(s, q * q, processType % 10);
    
    double integrand =  q * gsl_sf_bessel_J0( q * b); 

    if (calculateImagh)
	return   result.imag() * integrand;

    return   result.real() * integrand;
}

double TheoreticalModel::DrawFunction(double* x, double* par)
{
    SetProcessType((int)par[1]); 
    bool needst = processType / 100 == 3; 

    double energy = needst ? par[0] : x[0]; 
    double t =  ( !needst )? par[0] : x[0]; 

    SetParameters(par + 2); 

    double result = GetTheoreticalValue(energy, t);  
    return result; 
}



double g(double x, void * params)
{
  TheoreticalModel * computor = static_cast<TheoreticalModel *> (params);
  return computor -> Geta(x);
}

double f (double x, void * params) 
{
  TheoreticalModel * computor = static_cast<TheoreticalModel *> (params);
  return computor -> GetH(x);
}