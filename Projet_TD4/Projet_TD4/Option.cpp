#include "Option.h"
#include <cmath>
#include <iostream>

Option::Option()
{
	_n = 1;
	_time = 0.0;
}

Option::Option(double time, int n, double volatility, double riskFreeRate, double strike, double underlyingPrice)
{
	_time = time;
	_n = n;
	_volatility = volatility;
	_riskFreeRate = riskFreeRate;
	_strike = strike;
	_underlyingPrice = underlyingPrice;
}

Option::~Option()
{
	//
}

void Option::computeU()
{
	if (_deltaTime == 0)
		computeDeltaTime();
	_u = exp(_volatility * sqrt(_deltaTime));
	std::cout << "U = " << _u;
	std::cout << std::endl;
}

void Option::computeD()
{
	if (_u == 0)
		computeU();
	_d = 1 / _u;
	std::cout << "D = " << _d;
	std::cout << std::endl;
}

void Option::computeP()
{
	if (_u == _d) {
		computeU();
		computeD();
	}
	if (_u == _d)
		_p = 0;
	else
		_p = (exp(_riskFreeRate * _deltaTime) - _d) / (_u - _d);
	std::cout << "P = " << _p;
	std::cout << std::endl;
}

void Option::computeQ()
{
	if (_p == 0)
		computeP();
	_q = 1 - _p;
	std::cout << "Q = " << _q;
	std::cout << std::endl;
}

void Option::computeDeltaTime()
{
	_deltaTime = (double)_time / (double)_n;
	std::cout << "dT = " << _deltaTime;
	std::cout << std::endl;
}

void Option::computePrice()
{
	if (_u == 0) {
		computeU();
		computeD();
	}
	if (_p == 0) {
		computeP();
		computeQ();
	}
		

	if (_n == 0)
	{
		_n = 1;
	}
	if (_deltaTime == 0)
		computeDeltaTime();
	_optionPrice = 0;
	for (int i = 0; i <= _n; i++) {
		int a = factorial(i) * factorial(_n - i);
		if(a == 0)
			_optionPrice = _optionPrice + factorial(_n) / 1.0
			* power(_q, i)
			* power(1.0 - _q, _n - i)
			* payOff(i);
		else
		_optionPrice = _optionPrice + factorial(_n) / (factorial(i) * factorial(_n - i))
			* power(_q, i)
			* power(1.0 - _q, _n - i)
			* payOff(i);

	}
	_optionPrice = _optionPrice / power((1 + _riskFreeRate), _n);
}


double Option::getU()
{
	if (_u == 0)
		computeU();
	return _u;
}

double Option::getD()
{
	if (_d == 0)
		computeD();
	return _d;
}

double Option::getP()
{
	if (_p == 0)
		computeP();
	return _p;
}

double Option::getQ()
{
	if (_q == 0)
		computeQ();
	return _q;
}

double Option::getDeltaTime()
{
	return _deltaTime;
}

double Option::getOptionPrice()
{
	computePrice();
	return _optionPrice;
}

double Option::power(double a, int n)
{
	if (n <= 0) {
		return 1;
	}
	return a * power(a, n - 1);
}

int Option::factorial(int n)
{
	if (n <= 0) {
		return 1;
	}
	return n * factorial(n - 1);
}

double Option::max(double a, double b)
{
	if (a > b)
		return a;
	return b;
}

double Option::S_Ni(int i)
{
	if (_u == 0)
		computeU();
	if (_d == 0)
		computeD();
	return _underlyingPrice * power(_u, i) * power(_d, _n - i);
}

double Option::getStrike()
{
	return _strike;
}

EuropeanPUT::EuropeanPUT() : Option() {
	//
}

EuropeanPUT::EuropeanPUT(double time, int n, double volatility, double riskFreeRate, double strike, double underlyingPrice) :
	Option(time, n, volatility, riskFreeRate, strike, underlyingPrice) {
	//
}

EuropeanPUT::~EuropeanPUT() {
	//
}

double EuropeanPUT::payOff(int i) {
	return max(0, getStrike() - S_Ni(i));
}


EuropeanCALL::EuropeanCALL(): Option() {
	//
}

EuropeanCALL::EuropeanCALL(double time, int n, double volatility, double riskFreeRate, double strike, double underlyingPrice) :
	Option(time, n, volatility, riskFreeRate, strike, underlyingPrice) {
	//
}

EuropeanCALL::~EuropeanCALL() {
	//
}

double EuropeanCALL::payOff(int i) {
	return max(0, S_Ni(i) - getStrike());
}

DigitalPUT::DigitalPUT() : Option() {
	//
}

DigitalPUT::DigitalPUT(double time, int n, double volatility, double riskFreeRate, double strike, double underlyingPrice) :
	Option(time, n, volatility, riskFreeRate, strike, underlyingPrice) {
	//
}

DigitalPUT::~DigitalPUT() {
	//
}

double DigitalPUT::payOff(int i) {
	if (S_Ni(i) <  getStrike())
		return 1;
	return 0;
}

DigitalCALL::DigitalCALL() : Option() {
	//
}

DigitalCALL::DigitalCALL(double time, int n, double volatility, double riskFreeRate, double strike, double underlyingPrice) :
	Option(time, n, volatility, riskFreeRate, strike, underlyingPrice) {
	//
}

DigitalCALL::~DigitalCALL() {
	//
}

double DigitalCALL::payOff(int i) {
	if (S_Ni(i) > getStrike())
		return 1;
	return 0;
}