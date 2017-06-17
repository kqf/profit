#ifndef FITMANAGERMPI_H
#define	FITMANAGERMPI_H

#include "FitManager.h"
#include "Config.h"


class FitManagerMPI : public FitManager
{

public:
    double chi2(double * parameters);
    FitManagerMPI(int psize, int pid, bool test); 

protected:
    double ValueInPoint(const DataPoint & p);
    FitManagerMPI(const FitManagerMPI& orig) {}
    FitManagerMPI & operator=(const FitManagerMPI &) {}

private:
    int fPoolSize;
    int fProcID;
    bool fTest;
};

#endif	/* FITMANAGERMPI_H */

