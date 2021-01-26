#include "BlackScholesLimitBinomialTree.h"
#include <cmath>
#include <iostream>

// Default constructor
BlackScholesLimitBinomialTree::BlackScholesLimitBinomialTree()
{
	_n = 1;
	_maturity = 0.0;
}

// Constructor with parameters 
BlackScholesLimitBinomialTree::BlackScholesLimitBinomialTree(double riskFreeRate, double maturity, double strike, double underlyingPrice, double volatility, int n, BSOpType type)
{
	_riskFreeRate = riskFreeRate;
	_maturity = maturity;
	_strike = strike;
	_underlyingPrice = underlyingPrice;
	_n = n;
	_type = type;
	_volatility = volatility;
}

// Destructor 
BlackScholesLimitBinomialTree::~BlackScholesLimitBinomialTree()
{
	//
}

/* CLASSICAL COMPUTING U, D, P, Q */
void BlackScholesLimitBinomialTree::computeU()
{
	if (_deltaTime == 0)
		computeDeltaTime();
	_u = exp((_riskFreeRate + _volatility*_volatility/2)* _deltaTime + _volatility*sqrt(_deltaTime)) - 1;
	std::cout << _u << std::endl;
}

void BlackScholesLimitBinomialTree::computeD()
{
	if (_deltaTime == 0)
		computeDeltaTime();
	_d = exp((_riskFreeRate + _volatility * _volatility / 2) * _deltaTime - _volatility * sqrt(_deltaTime)) - 1;
	std::cout << _d << std::endl;
}

void BlackScholesLimitBinomialTree::computeP()
{
	if (_u == _d) {
		computeU();
		computeD();
		computeR();
	}
	if (_u == _d)
		_p = 0;
	else
		_p = (_r - _d) / (_u - _d);
	std::cout << _p <<std::endl;
}

void BlackScholesLimitBinomialTree::computeQ()
{
	if (_p == 0)
		computeP();
	_q = 1 - _p; 
	std::cout << _q << std::endl;
}

// Computing R
void BlackScholesLimitBinomialTree::computeR()
{
	if (_deltaTime == 0)
		computeDeltaTime();
	_r = exp(_riskFreeRate * _deltaTime) - 1;
	std::cout << _r << std::endl;
}

void BlackScholesLimitBinomialTree::computeDeltaTime()
{
	_deltaTime = _maturity / _n;
}

// Computing the price 
void BlackScholesLimitBinomialTree::computePrice()
{
	// Computing all the parameters if they are not computed 
	if (_u == 0)
		computeU();
	if (_d == 0)
		computeD();
	if (_p == 0)
		computeP();
	if (_q == 0)
		computeQ();
	if (_r == 0)
		computeR();
	if (_n == 0)
	{
		_n = 1;
	}
	if (_deltaTime == 0)
		computeDeltaTime();
	_optionPrice = 0;
	// Using the closed method to compute the price
	for (int i = 0; i <= _n; i++) {
		_optionPrice = _optionPrice  + factorial(_n)/(factorial(i)*factorial(_n - i)) *
			power(_p, i)
			* power(_q, _n - i)
			* payOff(i);
	}
	// Computing the price of the option 
	_optionPrice = _optionPrice / power((1 + _r), _n);
}

/* MATHEMATIC FUNCTIONS */
double BlackScholesLimitBinomialTree::power(double a, int n)
{
	if (n <= 0) {
		return 1;
	}
	return a * power(a, n - 1);
}

double BlackScholesLimitBinomialTree::factorial(int n)
{
	double fact = 1; 
	for (int i = 1; i <= n; i++) {
		fact = fact * i;
	}
	return fact;
}

double BlackScholesLimitBinomialTree::max(double a, double b)
{
	if (a > b)
		return a;
	return b;
}

// Computing the SN_i 
double BlackScholesLimitBinomialTree::S_Ni(int i)
{
	// Compute every parameters if not computed
	if (_u == _d) {
		computeU();
		computeD();
		computeR();
		computeP();
		computeQ();
	}
	// Return S0u^id^(n-i)
	return _underlyingPrice * power(_u, i) * power(_d, _n - i);
}

double BlackScholesLimitBinomialTree::payOff(int i)
{
	// Return the payoff
	if (_u == _d) {
		computeU();
		computeD();
		computeR();
		computeP();
		computeQ();
	}
	if (_type == BSCall)
		return max(S_Ni(i) - _strike, 0.0);
	return max(_strike - S_Ni(i), 0.0);
}

/* GETTERS */
double BlackScholesLimitBinomialTree::getU()
{
	if (_u = 0)
		computeU();
	return _u;
}

double BlackScholesLimitBinomialTree::getD()
{
	if (_d = 0)
		computeD();
	return _d;
}

double BlackScholesLimitBinomialTree::getP()
{
	if (_p = 0)
		computeP();
	return _p;
}

double BlackScholesLimitBinomialTree::getQ()
{
	if (_q = 0)
		computeQ();
	return _q;
}

double BlackScholesLimitBinomialTree::getDeltaTime()
{
	if (_deltaTime = 0)
		computeDeltaTime();
	return _deltaTime;
}

double BlackScholesLimitBinomialTree::getOptionPrice()
{
	if (_optionPrice == 0)
		computePrice();
	return _optionPrice;
}

double BlackScholesLimitBinomialTree::getStrike()
{
	return _strike;
}


