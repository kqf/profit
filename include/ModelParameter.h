/* 
 * File:   ModelParameter.h
 * Author: sha
 *
 * Created on July 14, 2014, 10:00 AM
 */

#ifndef MODELPARAMETER_H
#define	MODELPARAMETER_H

#include <string>
using std::string; 

class ModelParameter {
public:
    ModelParameter(const char* n, double v, double step, double a, double b);

    string name; 
    double value; 
    double step_size; 
    double lower_bound; 
    double upper_bound; 
};

#endif	/* MODELPARAMETER_H */

