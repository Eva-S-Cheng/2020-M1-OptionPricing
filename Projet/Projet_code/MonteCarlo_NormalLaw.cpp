#include "MonteCarlo_NormalLaw.h"

MonteCarlo_NormalLaw::MonteCarlo_NormalLaw(double lowerLimit, double upperLimit, double mean, double variance)
{
	_n = 0;
	_upper_limit = upperLimit;
	_lower_limit = lowerLimit;
	_mean = mean;
	_variance = variance;

	mt19937_singleton& gen = mt19937_singleton::getInstance();
	_gen = gen.getMT19937_gen();
}

// Destructor 
MonteCarlo_NormalLaw::~MonteCarlo_NormalLaw()
{
	// Nothing here
}

double MonteCarlo_NormalLaw::function(double x)
{
	// Function of a normal law (1/(s*sqrt(2pi))*exp(-1/2*((x-µ)/s)^2) where s is the standard deviation and µ the mean
	return (1 / (_variance * sqrt(2.0 * 3.14159265358))) * exp(-0.5 * ((x - _mean) / _variance) * ((x - _mean) / _variance));
}

void MonteCarlo_NormalLaw::compute(int numberOfPoints)
{
	/* LOCAL VARIABLES DECLARATION */
	std::uniform_real_distribution<> distribution(0.0, 1.0);					// Uniform distribution with real numbers (double, double, long double)
	double sum = 0.0;															// Initial value for the sum

	/* START */
	for (int i = 0; i < numberOfPoints; i++)
	{
		sum = sum + function(_lower_limit + distribution(_gen) * ((double)_upper_limit - (double)_lower_limit));
	}

	_currentEstimate = (sum * ((double)_upper_limit - (double)_lower_limit) + _currentEstimate * (double)_n) / ((double)numberOfPoints + (double)_n);
	_n = _n + numberOfPoints;
}

double MonteCarlo_NormalLaw::operator()()
{
	return _currentEstimate;
}