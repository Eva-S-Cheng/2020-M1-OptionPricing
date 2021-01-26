#include "MonteCarloOptionPricing.h"
#include <random>
#include <cmath>

// Computes the price of the asset
void MonteCarloOptionPricing::computeH0()
{
	if (_expectedPayOff == 0)
	{ 
		computeExpectedPayOff();
	}
	_h0 = exp(-_riskFreeRate * _maturity) * _expectedPayOff;
}

// Default Constructor
MonteCarloOptionPricing::MonteCarloOptionPricing()
{
	_riskFreeRate = 0.0;
	_maturity = 0.0;
	_strike = 0.0;
	_underlyingPrice = 0.0;
	_numberOfSteps = 0;
	_n = 0;
	_volatility = 0;
}

// Constructor with parameters 
MonteCarloOptionPricing::MonteCarloOptionPricing(double riskFreeRate, double maturity, double strike, double underlyingPrice, double volatility, int numberOfSteps, int n, opType type)
{
	_riskFreeRate = riskFreeRate;
	_maturity = maturity;
	_strike = strike;
	_underlyingPrice = underlyingPrice;
	_numberOfSteps = numberOfSteps;
	_n = n;
	_type = type;
	_volatility = volatility;
}

// Destructor
MonteCarloOptionPricing::~MonteCarloOptionPricing()
{
	//
}

// It computes the standard deviation of the payoffs 
void MonteCarloOptionPricing::computeStandardDeviation()
{
	if (_expectedPayOff == 0) {
		refine(0);
	}
	double sum = 0.0;
	for (int i = 0; i < _n; i++) {
		sum = _hT.at(i)*exp(-_riskFreeRate*_maturity) * _hT.at(i) * exp(-_riskFreeRate * _maturity) + sum;
	}
	if(_n !=0)
		sum = sum / _n;
	// Absolute value to avoid errors 
	_std = sqrt(abs(sum - exp(-_riskFreeRate * _maturity) * _expectedPayOff * _expectedPayOff * exp(-_riskFreeRate * _maturity)));
}

/* GETTERS */
double MonteCarloOptionPricing::getH0()
{
	// If it is not computed
	if (_h0 == 0)
		computeH0();
	return _h0;
}

double MonteCarloOptionPricing::getRate()
{
	return _riskFreeRate;
}

double MonteCarloOptionPricing::getMaturity()
{
	return _maturity;
}

double MonteCarloOptionPricing::getStrike()
{
	return _strike;
}

double MonteCarloOptionPricing::getVolatility()
{
	return _volatility;
}

double MonteCarloOptionPricing::getUnderlyingPrice()
{
	return _underlyingPrice;
}

double MonteCarloOptionPricing::getNumberOfSteps()
{
	return _numberOfSteps;
}

double MonteCarloOptionPricing::getAverage()
{
	return _expectedPayOff;
}

double MonteCarloOptionPricing::getSTD()
{
	return _std;
}

double MonteCarloOptionPricing::getN()
{
	return _n;
}

double MonteCarloOptionPricing::max(double a, double b)
{
	if (a < b)
		return b;
	return a;
}

// Refining the method
void MonteCarloOptionPricing::refine(int number)
{
	_n = _n + number;
	// Clearing the vectors
	_Si_t.clear();
	_hT.clear();
	// Computing again
	computeH0();
}

// Confindence interval for 95% = [mean +/- 2 * std/sqrt(n)]
double MonteCarloOptionPricing::uBoundConfInterval()
{
	return _h0 + _std * 2 / sqrt(_n);
}

double MonteCarloOptionPricing::dBouldConfInterval()
{
	return _h0 - _std * 2 / sqrt(_n);
}

// Default constructor 
EuropeanMonteCarloPricing::EuropeanMonteCarloPricing():MonteCarloOptionPricing()
{
	//
}

// Constructor with parameters
EuropeanMonteCarloPricing::EuropeanMonteCarloPricing(double riskFreeRate, double maturity, double strike, double underlyingPrice, double volatility, int n, opType type) :
	MonteCarloOptionPricing(riskFreeRate, maturity, strike, underlyingPrice, volatility, 1, n, type)
{
	//
}

// Destructor
EuropeanMonteCarloPricing::~EuropeanMonteCarloPricing()
{
	//
}

// The Wiener process follows a normal law with mean 0 and std t
double EuropeanMonteCarloPricing::WienerProcess(double x)
{
	return exp(-x * x / (2 * _maturity)) / (sqrt(3.14159265358 * 2 * _maturity));
}

// Sum of the payOff divided by the number of interation 
void EuropeanMonteCarloPricing::computeExpectedPayOff()
{
	createHT();
	double sum = 0.0;
	for (int i = 0; i < _n; i++)
	{
		sum = _hT.at(i) + sum;
	}
	_expectedPayOff = sum / _n;
}

// Create the vector that contains the price at maturity
void EuropeanMonteCarloPricing::createSi_t()
{
	// The x follows a normal law of parameters 0 and 1
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(0.0, 1.0);
	for (int i = 0; i < _n; i++)
	{
		std::vector<double> S_i;
		double x = distribution(generator);
		// Formula of the Price by using Wiener process
		double S_t = _underlyingPrice * exp((_riskFreeRate - _volatility * _volatility / 2.0) * _maturity + _volatility * WienerProcess(x));
		// Putting the estimation in the vector that contains the prices 
		S_i.push_back(S_t);
		_Si_t.push_back(S_i);
		S_i.clear();
	}
}

void EuropeanMonteCarloPricing::createHT()
{
	// Creating the prices 
	createSi_t();
	for (int i = 0; i < _n; i++)
	{
		// Computes the payoffs
		if(_type == CallOP)
			_hT.push_back(max(_Si_t.at(i).at(0) - _strike,0));
		else 
			_hT.push_back(max(_strike - _Si_t.at(i).at(0), 0));
	}
}

// Default Constructor
AsianMonteCarloPricing::AsianMonteCarloPricing():MonteCarloOptionPricing()
{
	//
}

// Constructor with parameters 
AsianMonteCarloPricing::AsianMonteCarloPricing(double riskFreeRate, double maturity, double strike, double underlyingPrice, double volatility, int numberOfSteps, int n, opType type) :
	MonteCarloOptionPricing(riskFreeRate, maturity, strike, underlyingPrice, volatility, numberOfSteps, n, type)
{
	//
}
AsianMonteCarloPricing::~AsianMonteCarloPricing()
{
	//
}

// Create the vector that contains the pay offs
void AsianMonteCarloPricing::createHT()
{
	createSi_t();
	for (int i = 0; i < _n; i++)
	{
		double sum = 0.0;
		// Mean of the prices 
		for (int j = 0; j < _numberOfSteps; j++)
		{
			sum = _Si_t.at(i).at(j) + sum;
		}
		sum = sum / _numberOfSteps;

		// Pay off between the average and the stike 
		if (_type == CallOP)
			_hT.push_back(max(sum - _strike, 0.0));
		else
			_hT.push_back(max(_strike - sum, 0.0));
	}
}

void AsianMonteCarloPricing::computeExpectedPayOff()
{
	// Average of the Pay Off
	createHT();
	double sum = 0.0;
	for (int i = 0; i < _n; i++)
	{
		sum = _hT.at(i) + sum;
	}
	_expectedPayOff = sum / _n;
}

void AsianMonteCarloPricing::createSi_t()
{
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(0.0, 1.0);
	for (int i = 0; i < _n; i++) {
		// S_t at t - 1
		double St_kminusone = _underlyingPrice;
		std::vector<double> toBePushedBack;
		for (int j = 0; j < _numberOfSteps; j++) {
			// z_k that follow a normal law 
			double z_k = distribution(generator);
			// Formula of the S_tk
			double St_k = St_kminusone*exp((_riskFreeRate-_volatility*_volatility/2)*(((j+1)*_maturity/_numberOfSteps)-(j*_maturity/_numberOfSteps))+_volatility*
				sqrt(((j + 1) * _maturity / _numberOfSteps) - (j * _maturity / _numberOfSteps))*z_k);
			// Adding the price to the vector 
			toBePushedBack.push_back(St_k);
			// St_k at k - 1 become St_k at k
			St_kminusone = St_k;
		}
		// Adding the vector of vectors
		_Si_t.push_back(toBePushedBack);
		// Clearing the vector
		toBePushedBack.clear();
	}
}
