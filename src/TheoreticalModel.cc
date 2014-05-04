/* 
 * File:   TheoreticalModel.cc
 * Author: sha
 * 
 * Created on May 4, 2014, 3:20 PM
 */

#include "TheoreticalModel.h"

TheoreticalModel::TheoreticalModel() 
{
    npars = 12; 
}

TheoreticalModel::TheoreticalModel(const TheoreticalModel& orig) { }

TheoreticalModel::~TheoreticalModel() { }

double TheoreticalModel::DrawFunction(double* x, double* par) const
{
    return 40; 
}

