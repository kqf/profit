/* 
 * File:   TheoreticalModel.h
 * Author: sha
 *
 * Created on May 4, 2014, 3:20 PM
 */

#ifndef THEORETICALMODEL_H
#define	THEORETICALMODEL_H

class TheoreticalModel {
public:
    TheoreticalModel();
    TheoreticalModel(const TheoreticalModel& orig);
    virtual ~TheoreticalModel();
    double DrawFunction(double * x, double * par) const; 
    int npars; 
private:

};

#endif	/* THEORETICALMODEL_H */

