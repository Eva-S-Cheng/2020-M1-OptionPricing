#pragma once
#include "mt19937_singleton.h"

class MonteCarlo_NormalLaw						// Monte Carlo approach for the normal law
{
private:
	double _mean;								
	double _variance;							

	int _n;										// The number of points that are generated
	double _upper_limit, _lower_limit;
	double _currentEstimate;					// Updated by the compute method
	std::mt19937 _gen;							// mt19937 singleton 

public:
	MonteCarlo_NormalLaw(double, double, double mean = 0, double variance = 1);
	~MonteCarlo_NormalLaw();
	double function(double numberOfPoints);		// Normal law equation here

	double operator()();						// parenthesis operator, they return the estimation of the Integral
	void compute(int);							// Computing the monte carlo integral	
};

