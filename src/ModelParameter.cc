/* 
 * File:   ModelParameter.cc
 * Author: sha
 * 
 * Created on July 14, 2014, 10:00 AM
 */

#include "ModelParameter.h"

ModelParameter::ModelParameter(const char* n, double v, double s,
                                                        double a, double b):
        name(n),
	value(v),
	step_size(s), 
	lower_bound(a),
	upper_bound(b)
{

}