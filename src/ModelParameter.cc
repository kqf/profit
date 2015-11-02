/* 
 * File:   ModelParameter.cc
 * Author: sha
 * 
 * Created on July 14, 2014, 10:00 AM
 */

#include "ModelParameter.h"
#include <limits>
#include <iostream>
#include <iomanip>


ModelParameter::ModelParameter(const char* n, double v, double s,
                                                        double a, double b):
    name(n),
	value(v),
	step_size(s), 
	lower_bound(a),
	upper_bound(b)
{

}

std::ifstream & ignore_comment(std::ifstream & in)
{
    std::cout << char(in.peek()) << std::endl;
    if(in.peek() == '#')
    {
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "ignoring" << std::endl;

    }
    return in;
}

std::vector<ModelParameter> ModelParameter::GetParameters(const char * filename)
{
    std::ifstream fin(filename);

    string n;
    double val;
    double st;
    double a;
    double b;


    std::vector<ModelParameter> parameters;
    while (ignore_comment(fin) >> n >> val >> st >> a >> b >> std::ws)
        parameters.push_back( ModelParameter(n.c_str(), val, st, a, b) );

    return  parameters;
}
std::ostream & operator<<(std::ostream & stream, const ModelParameter & p)
{
    char separator = ' ';
    stream  << std::setw(12) <<  std::setfill(separator) << p.name 
            << std::setw(12) <<  std::setfill(separator) << p.value 
            << std::setw(12) <<  std::setfill(separator) << p.step_size
            << std::setw(12) <<  std::setfill(separator) << p.lower_bound   
            << std::setw(12) <<  std::setfill(separator) << p.upper_bound << std::endl;
    return stream;
}
