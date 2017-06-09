/*
 * File:   PhysicalProcess.cc
 * Author: sha
 *
 * Created on May 4, 2014, 9:38 AM

    NB: Due to naming convention use this->... in this file. Change it later (someday).

 */
#include <iostream>

#include "PhysicalProcess.h"


#include <TChain.h>

PhysicalProcess::PhysicalProcess(const char* process_name, int code, const char* graph_title):
    chi2(0)
{
    name = process_name;
    dataCode = code;
    title = graph_title;
    numberOfpoints = 0;
}


void PhysicalProcess::FillFromFile(const char * filename)
{
    TChain * dat = new TChain(this->name);
    dat->Add(filename);
    float a, b , c, d;

    dat ->SetBranchAddress("intensity", &a);
    dat ->SetBranchAddress("error", &b);
    dat ->SetBranchAddress("energy", &c);
    dat ->SetBranchAddress("t", &d);
    for (int i = 0 ; i < dat->GetEntries(); ++i )
    {
        dat->GetEntry(i);
        this->experimentalPoints.push_back(DataPoint((double)a, (double)b, (double)c, (double)d));

        DataPoint & point = this->experimentalPoints[i];
        if (this->AcceptDataPoint(point))
            point.ignore = true;
    }
    this->numberOfpoints = this->experimentalPoints.size();

    std::cout << "Number of points for " << this->name << " : " <<  this->experimentalPoints.size() << std::endl;

    delete dat;
    dat = 0;
}



// TODO: Move all selection to separate test (maybe python script ...)
//

int convert_energy(const double & en)
{
    return int(1000 * en);
}

bool PhysicalProcess::AcceptDataPoint(const DataPoint& p) const
{
    double lower_t = 0.01;
    double upper_t = 9;
    double lower_energy = 19;
    double upper_energy = 2000;

    double lower_energy_sigma = 5;
    double upper_energy_sigma = 2000;

    int exclude_energy = convert_energy(52.818);

    double aexclude = 52.79;
    double bexclude = 52.85;

    bool is_valid = false;
    if (this->dataCode > 300)
        is_valid  = ( (p.energy > lower_energy) && (p.energy < upper_energy) ) &&
                    ( (p.t > lower_t) && (p.t < upper_t));
    else
        is_valid =  ( (p.energy > lower_energy_sigma) && (p.energy < upper_energy_sigma) ) ;

    //    if (convert_energy(p.energy) == exclude_energy)
    //  return false;

    if ( ( (p.energy > aexclude) && (p.energy < bexclude) ) && this->dataCode == 310 )
        return true;

    // true include data point, false exclude
    return !is_valid;
}

