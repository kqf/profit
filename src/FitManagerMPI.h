#ifndef FITMANAGERMPI_H
#define	FITMANAGERMPI_H

#include "FitManager.h"


// TODO: Add separate test for the FitManagerMPI

class FitManagerMPI : public FitManager
{

public:
    double chi2(const double * parameters);
    FitManagerMPI(int psize, int pid); 

protected:
    FitManagerMPI(const FitManagerMPI& orig) {}
    FitManagerMPI & operator=(const FitManagerMPI &) {}

private:
    int pool_size;
    int procid;
};

#endif	/* FITMANAGERMPI_H */

