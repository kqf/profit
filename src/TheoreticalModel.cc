/* 
 * File:   TheoreticalModel.cc
 * Author: sha
 * 
 * Created on May 4, 2014, 3:20 PM
 */

#include "TheoreticalModel.h"
#include "ModifiedPole.h"
#include "QuadraticPole.h"

#include <iostream>

#include <gsl/gsl_errno.h>

TheoreticalModel::TheoreticalModel(const double * par, int n):npars(n)
{
    // gsl_set_error_handler_off(); 
    gsl_set_error_handler_off(); 
    SetParameters(par); 
}

void TheoreticalModel::SetParameters(const double * par)
{
    QuadraticPole * pomeron = new QuadraticPole(par[0], par[1], par[2], par[3], par[4], par[5] < 0); 
    QuadraticPole * odderon = new QuadraticPole(par[6], par[1] - par[7], par[2] - par[8], par[9], par[10] - par[4], par[11] < 0); 

    int skipped = 12;

    poles = ReggePole::MakePoles(par + skipped , npars - 1 - skipped); 

    poles.push_back(pomeron);
    poles.push_back(odderon);

    // TODO: check correctness of 2 * i * lambda
    ilambda = complexd(0, 2 * par[npars - 1]); 

    // We set new parameters, result is interesting
    // We do need evaluation
    skipEvaluation = false;
}

double TheoreticalModel::GetTheoreticalValue(double  energy
					    ,double  transverse_momentum)
{
    assert(npars != 0 && "Parameters are not set");
    if (skipEvaluation) return 0.; // our model contains bad parameters -- just quit
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
    // bad model parameters, don't waste time for integration
    // Can't put 0 here since Rho gives nan.
    if(skipEvaluation) return 0.001; 

    double result = 0;
    double error  = 0;
    double precision_abs = 1e-3;
    double precision_rel = 1e-3;
    int workspace = 1e+4;
    
    
    // TODO: remove this invocation from here!!!
    gsl_integration_workspace * w = gsl_integration_workspace_alloc (workspace);
    gsl_function F;

    F.function = function;
    F.params = (void *) this;
    int status = 0;
 
    if(whole_range)
    	status = gsl_integration_qagiu (&F, 0, precision_abs, precision_rel, workspace, w, &result, &error); 
    else
        status = gsl_integration_qags  (&F, 0, 30, precision_abs, precision_rel, workspace, w, &result, &error); 

    if (status == GSL_ESING)
    {
        PrintFailure(status);
        return 0.;
    }

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
	result += poles[i]->Amplitude(s, q * q, processType % 10);
    
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
void TheoreticalModel::PrintFailure(const int & status)
{
    // Once we see problem in integrals 
    // we should skip further computations until we change parameters
    skipEvaluation = true;


    std::cout << "Problem with integration " <<  gsl_strerror(status)  << std::endl;
    for(int i = 0; i < poles.size(); ++i)
        poles[i]->PrintParameters();
    std::cout << "Lambda " << ilambda  << std::endl;

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
