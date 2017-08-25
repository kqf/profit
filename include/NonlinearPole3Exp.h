#ifndef NONLINLINEEARPOLE3EXP
#define NONLINLINEEARPOLE3EXP

#include <iostream>
#include <iomanip>

#include <complex>
#include <cassert>

#include "AbstractPole.h"

class NonlinearPole3Exp : public AbstractPole
{
public:
	static Poles MakePoles(const double *, const int &, int &);

	virtual void PrintParameters() const
	{

		std::cout << "alp1"     << std::setw(8) << alp1
		          << "\talp1p"  << std::setw(8) << alp1p
		          << "\tbetap1" << std::setw(8) << betap1
		          << "\tbetap2" << std::setw(8) << betap2
		          << "\tbetap3" << std::setw(8) << betap3
		          << "\tcp1"    << std::setw(8) << cp1
		          << "\tcp2"    << std::setw(8) << cp2
		          << "\tcp3"    << std::setw(8) << cp3
		          << "\tOdd  "  << std::setw(8) << isOdd << std::endl;
	}

	virtual void SetParameters(const double * pars, int & offset);

	NonlinearPole3Exp(const double & a, const double & b,
	                  const double & c, const double & d,
	                  const double & e, const double & f,
	                  const double & j, const double & k,
	                  bool odd):

		alp1   (a),
		alp1p  (b),
		betap1 (c),
		betap2 (d),
		betap3 (e),
		cp1    (f),
		cp2    (j),
		cp3    (k),
		AbstractPole(odd)
	{
		/*PrintParameters();*/
	}

	NonlinearPole3Exp(const double * p, int & offset):
		alp1   (p[offset]),
		alp1p  (p[offset + 1]),
		betap1 (p[offset + 2]),
		betap2 (p[offset + 3]),
		betap3 (p[offset + 4]),
		cp1    (p[offset + 5]),
		cp2    (p[offset + 6]),
		cp3    (p[offset + 7]),
		AbstractPole(p[offset + 8]  < 0)
	{
		offset += kInputParameters;
	}

	// this is number of input parameters in constructor!
	// if you modify the constructor --- you should modify
	// this number too !!
	static const int kInputParameters = 9;


private:

	virtual complexd PureAmplitude(const double & s, const double & t) const ;

	double alp1;
	double alp1p;
	double betap1;
	double betap2;
	double betap3;
	double cp1;
	double cp2;
	double cp3;
};

#endif  // NONLINLINEEARPOLE3EXP 