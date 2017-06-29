/*
 * File:   NonlinearPoleT2V3.h
 * Author: sha
 *
 * Created on May 19, 2014, 8:01 PM
 */

#ifndef NONLINEARPOLET2V3_H
#define NONLINEARPOLET2V3_H

#include <iostream>
#include <iomanip>

#include <complex>
#include <cassert>

#include "AbstractPole.h"

class NonlinearPoleT2V3 : public AbstractPole
{
public:
//    complexd Amplitude(const double & s, const double & t, bool) const;
	static Poles MakePoles(const double *, const int &, int &);

	virtual void PrintParameters() const
	{
		std::cout << "jja0   " << std::setw(8) << a0
		          << "\tgamma" << std::setw(8) << gamma
		          << "\ttr   " << std::setw(8) << tr
		          << "\tg    " << std::setw(8) << g
		          << "\tB    " << std::setw(8) << B
		          << "\tTau  " << std::setw(8) << Tau
		          << "\tC    " << std::setw(8) << C
		          << "\teta  " << std::setw(8) << eta
		          << "\tnu   " << std::setw(8) << nu
		          << "\tOdd  " << std::setw(8) << isOdd << std::endl;
	}

	virtual void SetParameters(const double * pars, int & offset);

	NonlinearPoleT2V3(const double & a, const double & b,
	                  const double & c, const double & d,
	                  const double & e, const double & f,
	                  const double & j, const double & k,
	                  const double & l, bool odd):

		a0(a), gamma(b), tr(c), g(d), B(e), Tau(f), C(j), eta(k), nu(l), AbstractPole(odd) {/*PrintParameters();*/}

	NonlinearPoleT2V3(const double * p, int & offset):
		a0(   p[offset]),
		gamma(p[offset + 1]),
		tr(   p[offset + 2]),
		g(    p[offset + 3]),
		B(    p[offset + 4]),
		Tau(  p[offset + 5]),
		C(    p[offset + 6]),
		eta(  p[offset + 7]),
		nu(   (p[offset + 8] < 0) ? 0 : p[0]),
		AbstractPole(p[offset + 8] < 0)
	{
		offset += kInputParameters - 1;
	}

	// this is number of input parameters in constructor!
	// if you modify the constructor --- you should modify
	// this number too !!
	static const int kInputParameters = 10;


private:

	virtual complexd PureAmplitude(const double & s, const double & t) const ;

	double a0;
	double gamma;
	double tr;
	double g;
	double Tau;
	double B;
	double C;
	double nu;
	double eta;
};

#endif  /* NONLINEARPOLET2V3_H */