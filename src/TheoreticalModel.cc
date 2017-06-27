/*
 * File:   TheoreticalModel.cc
 * Author: sha
 *
 * Created on May 4, 2014, 3:20 PM
 */

#include "TheoreticalModel.h"
#include "ModifiedPole.h"
#include "QuadraticPole.h"
#include "NonlinearPoleT2V3.h"

#include <iostream>

#include <gsl/gsl_errno.h>

TheoreticalModel::TheoreticalModel(const double * par, int n): npars(n)
{
    gsl_set_error_handler_off();
    SetParameters(par);
}

void TheoreticalModel::SetParameters(const double * par)
{
    // TODO: Improve this part somehow
    // 

    double nu = par[0];
    int skipped = 1;
    // int skipped = 0;

    AbstractPole * pomeron1 = NonlinearPoleT2V3::MakeNonlinearPole(par + skipped, nu);
    skipped = skipped + NonlinearPoleT2V3::nImputParamets - 1;

    AbstractPole * pomeron2 = NonlinearPoleT2V3::MakeNonlinearPole(par + skipped, nu);
    skipped = skipped + NonlinearPoleT2V3::nImputParamets - 1;

    // std::cout << pomeron1->Amplitude(0.194200e+02 * 0.194200e+02, -0.389564e-02, false) << std::endl;
    // return;

    AbstractPole * odderon1 = NonlinearPoleT2V3::MakeNonlinearPole(par + skipped, 0);
    skipped = skipped + NonlinearPoleT2V3::nImputParamets - 1;

    fPoles = ReggePole::MakePoles(par + skipped , npars - 1 - skipped);
    // fPoles = ReggePole::MakePoles(par + skipped , 0);
    fPoles.push_back(pomeron1);
    fPoles.push_back(pomeron2);
    // fPoles.push_back(pomeron3);

    fPoles.push_back(odderon1);
    // fPoles.push_back(odderon2);

    // TODO: check correctness of 2 * i * lambda
    fIlambda = complexd(0, 2 * par[npars - 1]);

    // We set new parameters, result is interesting
    // We do need evaluation
    fSkipEvaluation = false;
}

double TheoreticalModel::GetTheoreticalValue(double  energy , double  transverse_momentum) const
{
    assert(npars != 0 && "Parameters are not set");
    if (fSkipEvaluation)
        return 0.; // our model contains bad parameters -- just quit

    double s = energy * energy;
    double t = transverse_momentum;

    double k = 0.3893797;
    double mp = 0.938270;

    int observableType = int(fProcessId / 100);

    complexd A = GetA(s, t, observableType == 1);  // only 110 and 111 need only image part
    switch (observableType)
    {
    case 1:
        return k * (8 * M_PI) * A.imag();
    case 2:
        return A.real() / A.imag();
    case 3:
        return std::norm(A) * k * 4 * M_PI ;//  * (8 * M_PI * s) * k / ( 16 * M_PI * s  )  ;
    default:
        assert(true && "Unexpected observable type");
    }

    return 0;
}

TheoreticalModel::complexd TheoreticalModel::GetA(double s, double t, bool imag) const
{
    IntegrationParameters pars(this, s, t, true);
    double im  = BesselTransform(f, &pars, 25.);

    pars.fImag = false;
    double re = imag ? 0. : BesselTransform(f, &pars, 25.);

    return complexd(re, im);
}


double TheoreticalModel::GetH(double b, double s, double t, bool imag) const
{
    IntegrationParameters pars(this, s, b, false);
    double realh = BesselTransform(g, &pars);

    pars.fImag = true;
    double imagh = BesselTransform(g, &pars);

    complexd h = std::complex<double >(realh, imagh) / (8. * M_PI * s);
    complexd H = Unitarize(h);

    double integrand =  b * gsl_sf_bessel_J0(sqrt(t) * b);
    return integrand * (imag ? H.imag() : H.real());
}

TheoreticalModel::complexd TheoreticalModel::Unitarize(const complexd& h) const
{
    // return h / ( -0.5 * fIlambda * h + std::complex<double >(1, 0) );
    return (exp(fIlambda * h ) - 1.) / fIlambda;
}

double TheoreticalModel::Geta(double t, double s, double b, bool imag) const
{
    complexd result = 0;

    for (int i = 0; i < fPoles.size(); ++i)
        result += fPoles[i]->Amplitude(s, t * t, fOddProcess);

    double integrand = t * gsl_sf_bessel_J0(t * b);
    return integrand * (imag ? result.imag() : result.real());
}

double TheoreticalModel::DrawFunction(double* x, double* par)
{
    SetProcessType((int)par[1]);
    bool needst = fProcessId / 100 == 3;

    std::cout << fProcessId << std::endl;

    double energy = needst ? par[0] : x[0];
    double t = (!needst) ? par[0] : x[0];

    SetParameters(par + 2);

    // return 0;
    double result = GetTheoreticalValue(energy, t);
    return result;
}
void TheoreticalModel::PrintFailure(const int & status)
{
    // Once we see problem in integrals
    // we should skip further computations until we change parameters
    fSkipEvaluation = true;


    std::cout << "Problem with integration " <<  gsl_strerror(status)  << std::endl;
    for (int i = 0; i < fPoles.size(); ++i)
        fPoles[i]->PrintParameters();
    std::cout << "Lambda " << fIlambda  << std::endl;

}

double g(double t, void * params)
{
    IntegrationParameters * p = static_cast<IntegrationParameters *> (params);

    // Partial amplitude a(t, s, b)
    return p->fModel->Geta(t, p->fS, p->fY, p->fImag);
}

double f (double b, void * params)
{
    IntegrationParameters * p = static_cast<IntegrationParameters *> (params);

    // Impact amplitude H(b, s, t)
    return p->fModel->GetH(b, p->fS, p->fY, p->fImag);
}
