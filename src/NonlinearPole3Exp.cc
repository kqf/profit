#include "NonlinearPole3Exp.h"
#include <iostream>


NonlinearPole3Exp::complexd NonlinearPole3Exp::PureAmplitude(const double & s, const double & t) const
{

    complexd ss(0, -s);
    complexd alphalog = log(ss) * alp1p;

    complexd r1 = betap1 + alphalog;
    complexd r2 = betap2 + alphalog;
    complexd r3 = betap3 + alphalog;

    complexd first  = cp1 * exp(-0.25 * t * t / r1) * (0.5 / r1);
    complexd second = cp2 * exp(-0.25 * t * t / r2) * (0.5 / r2);
    complexd third  = cp3 * exp(-0.25 * t * t / r3) * (0.5 / r3);


    complexd fbp1 = first + second + third;
    double coef = 1.;
    complexd result = fbp1 * pow(ss, alp1) * coef;
    return result;


}

// This funciton creates needed number of poles:
// n -- control number of poles in a model
//
//

AbstractPole::Poles NonlinearPole3Exp::MakePoles(const double * p, const int & npoles, int & offset)
{
    AbstractPole::Poles poles;
    for (int i = 0; i < npoles; ++i)
    {
        AbstractPole * pole = new NonlinearPole3Exp(p[offset], p[offset + 1], p[offset + 2], p[offset + 3],
                p[offset + 4], p[offset + 5], p[offset + 6], p[offset + 7], p[offset + 8] < 0 );

        poles.push_back(std::shared_ptr<AbstractPole>(pole));
        offset += kInputParameters;
    }

    return poles;
}


void NonlinearPole3Exp::SetParameters(const double * pars, int & offset)
{
    alp1   = pars[offset]; 
    alp1p  = pars[offset + 1];
    betap1 = pars[offset + 2];
    betap2 = pars[offset + 3];
    betap3 = pars[offset + 4];
    cp1    = pars[offset + 5];
    cp2    = pars[offset + 6];
    cp3    = pars[offset + 7];
    isOdd  = pars[offset + 8] < 0; 
    // Don't change the internal properties
    // odd = ...

    // Update the offset value
    offset += kInputParameters  - 1;
}

