#pragma once
#include "mt19937_singleton.h"

class MonteCarlo_NormalLaw
{
private:
	double _mean;										// Mean, by default = 
	double _variance;									// Standard deviation, by default = 1

	int _n;												// The number of points already generated
	double _upper_limit, _lower_limit;
	double _currentEstimate;								// Updated by the compute method
	std::mt19937 _gen;

public:
	MonteCarlo_NormalLaw(double, double, double mean = 0, double variance = 1);
	~MonteCarlo_NormalLaw();
	double function(double numberOfPoints);				// Normal law equation here

	double operator()();									// parenthesis operator, they return the estimation of the Integral
	void compute(int);									
};

