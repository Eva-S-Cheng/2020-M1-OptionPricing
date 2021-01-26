#include "CRRPricer_ClosedMethod.h"
#include <cmath>
#include <iostream>

// Default constructor
CRRPricer_ClosedMethod::CRRPricer_ClosedMethod() {
	_n = 1;
	_time = 0;
}

// Constructor with parameter 
CRRPricer_ClosedMethod::CRRPricer_ClosedMethod(double time, int n, double volatility,
	double riskFreeRate, double strike, double underlyingPrice, optionName optionType) {
	_time = time;
	_n = n;
	_volatility = volatility;
	_riskFreeRate = riskFreeRate;
	_strike = strike;
	_underlyingPrice = underlyingPrice;
	_optionType = optionType;
};

// Computing the delta time 
void CRRPricer_ClosedMethod::computeDeltaTime() {
	_deltaTime = (double)_time / (double)_n;
}

// Computing u
void CRRPricer_ClosedMethod::computeU() {
	if (_deltaTime == 0)
		computeDeltaTime();
	_u = exp(_volatility * sqrt(_deltaTime));
	std::cout << "U = " << _u;
	std::cout << std::endl;
}

// Computing d
void CRRPricer_ClosedMethod::computeD() {
	if (_u == 0)
		computeU();
	_d = 1 / _u;
	std::cout << "D = " << _d;
	std::cout << std::endl;
}

// Computing the probability of going up
void CRRPricer_ClosedMethod::computeP() {
	// If u = d, they haven't been computed yet
	if (_u == _d) {
		computeU();
		computeD();
	}
	if (_u == _d)
		_p = 0;
	// p
	else
		_p = (exp(_riskFreeRate * _deltaTime) - _d) / (_u - _d);
	std::cout << "P = " << _p;
	std::cout << std::endl;

}

void CRRPricer_ClosedMethod::computeQ() {
	// If p = 0 it means that there is an important probability that p hasn't been computed
	if (_p == 0)
		computeP();

	// q = 1 - p
	_q = 1.0 - _p;
	std::cout << "Q = " << _q;
	std::cout << std::endl;
}


void CRRPricer_ClosedMethod::computeOptionPrice() {
	// Computing all the parameters 
	if (_u == 0)
		computeU();
	if (_d == 0)
		computeD();
	if (_p == 0)
		computeP();
	if (_q == 0)
		computeQ();

	if (_n == 0)
	{
		_n = 1;
	}
	if (_deltaTime == 0)
		computeDeltaTime();

	// Using the closed method to compute the price
	if (_optionType == PUT) {
		for (int i = 0; i <= _n; i++) {
			_optionPrice = _optionPrice + factorial(_n) / (factorial(i) * factorial(_n - i))
				* power(_q, i)
				* power(1.0 - _q, _n - i)
				* max(_strike - S_Ni(i), 0);

		}
	}

	if (_optionType == CALL) {
		for (int i = 0; i <= _n; i++) {
			_optionPrice = _optionPrice + factorial(_n) / (factorial(i) * factorial(_n - i))
				* power(_q, i)
				* power(1 - _q, _n - i)
				* max(S_Ni(i) - _strike, 0);
		}
	}
	_optionPrice = _optionPrice / power((1 + _riskFreeRate), _n);
}

/* GETTERS */
double CRRPricer_ClosedMethod::getU() {
	if (_u == 0)
		computeU();
	return _u;
}

double CRRPricer_ClosedMethod::getD() {
	if (_d == 0)
		computeD();
	return _d;
}

double CRRPricer_ClosedMethod::getP() {
	if (_p == 0)
		computeP();
	return _p;
}

double CRRPricer_ClosedMethod::getQ() {
	if (_q == 0)
		computeQ();
	return _q;
}

double CRRPricer_ClosedMethod::getDeltaTime() {
	if (_deltaTime == 0)
		computeDeltaTime();
	return _deltaTime;
}

double CRRPricer_ClosedMethod::getOptionPrice() {
	if (_optionPrice == 0)
		computeOptionPrice();
	return _optionPrice;
}

/* CLASSICAL MATHEMATIC FUCNTIONS DEFINED RECURSIVELY */
double CRRPricer_ClosedMethod::power(double a, int n) {
	if (n <= 0) {
		return 1;
	}
	return a * power(a, n - 1);
}

int CRRPricer_ClosedMethod::factorial(int n) {
	if (n <= 0) {
		return 1;
	}
	return n * factorial(n - 1);
}

double CRRPricer_ClosedMethod::max(double a, double b) {
	if (a > b)
		return a;
	return b;
}

// Computing the S_Ni
double CRRPricer_ClosedMethod::S_Ni(int i) {
	return _underlyingPrice * power(_u, i) * power(_d, _n - i);
}