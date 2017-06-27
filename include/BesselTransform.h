#ifndef BESSELTRANSFORM_H
#define BESSELTRANSFORM_H

#include "TheoreticalModel.h"
#include <gsl/gsl_integration.h>

class TheoreticalModel;

struct IntegrationParameters
{

    IntegrationParameters(const TheoreticalModel * m, double s, double y, bool i):
        fModel(m),
        fS(s),
        fY(y),
        fImag(i)
    {}

    const TheoreticalModel * fModel;
    double fS;
    double fY;
    bool fImag;
};

double BesselTransform(double (* function)(double, void*), IntegrationParameters * pars, double upper = -1);

#endif  /* BESSELTRANSFORM_H */
