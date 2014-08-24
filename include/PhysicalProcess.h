/* 
 * File:   PhysicalProcess.h
 * Author: sha
 *
 * Created on May 4, 2014, 9:38 AM
 */

#ifndef PHYSICALPROCESS_H
#define	PHYSICALPROCESS_H

#include <vector>

struct DataPoint
{
    DataPoint(): observable(0),error(0),energy(0),t(0),ignore(false) {}
    DataPoint(double a,double b): observable(a),error(b),energy(0),t(0),ignore(false) {}
    DataPoint(double a,double b, double c, double d): observable(a),error(b),energy(c),t(d),ignore(false) {}

    double observable; 
    double error; 
    double energy; 
    double t; 
    bool ignore; 
};

struct PhysicalProcess
{
    PhysicalProcess(const char * process_name, int code, const char * graph_title, int points); 
    double chi2;  // chi2 per process
    const char * name; 
    const char * title; 
    unsigned int numberOfpoints; 
    short int dataCode; 
    bool tIsArgument; 
    std::vector<DataPoint> experimentalPoints; 
};

#endif	/* PHYSICALPROCESS_H */

