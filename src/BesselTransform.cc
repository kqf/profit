#include "BesselTransform.h"

double BesselTransform(double (* function)(double, void*), IntegrationParameters * pars, double upper)
{
    // bad model parameters, don't waste time for integration
    // Can't put 0 here since Rho gives nan.
    double result = 0;
    double error  = 0;
    double precision_abs = 1e-3;
    double precision_rel = 1e-3;
    int workspace = 1e+3;


    // TODO: remove this invocation from here!!!
    gsl_integration_workspace * w = gsl_integration_workspace_alloc (workspace);
    gsl_function F;

    F.function = function;
    F.params = (void *) pars;
    int status = 0;

    if (upper < 0)
        status = gsl_integration_qagiu (&F, 0, precision_abs, precision_rel, workspace, w, &result, &error);
    else
        status = gsl_integration_qags  (&F, 0, upper, precision_abs, precision_rel, workspace, w, &result, &error);

    // if (status == GSL_ESING)
    // {
    //     PrintFailure(status);
    //     return 0.;
    // }
    
    gsl_integration_workspace_free (w);
    return result;
}

