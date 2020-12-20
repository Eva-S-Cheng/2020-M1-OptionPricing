#include "BlackScholesLimitBinomialTree.h"
#include <cmath>

void BlackScholesLimitBinomialTree::computeU()
{
	if (_deltaTime == 0)
		computeDeltaTime();
	_u = exp((_riskFreeRate + _volatility*_volatility/2)* _deltaTime + _volatility*sqrt(_deltaTime)) - 1.0;
}

void BlackScholesLimitBinomialTree::computeD()
{
	if (_deltaTime == 0)
		computeDeltaTime();
	_d = exp((_riskFreeRate + _volatility * _volatility / 2) * _deltaTime - _volatility * sqrt(_deltaTime)) - 1.0;
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
}

void BlackScholesLimitBinomialTree::computeQ()
{
	if (_p == 0)
		computeP();
	_q = 1 - _p; 

}

void BlackScholesLimitBinomialTree::computeR()
{
	if (_deltaTime == 0)
		computeDeltaTime();
	_r = exp(_riskFreeRate * _deltaTime) - 1;
}

void BlackScholesLimitBinomialTree::computeDeltaTime()
{
	_deltaTime = _maturity / _n;
}

void BlackScholesLimitBinomialTree::computePrice()
{
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

	// Using the closed method to compute the price
	for (int i = 0; i <= _n; i++) {
		_optionPrice = _optionPrice + factorial(_n) / (factorial(i) * factorial(_n - i))
			* power(_q, i)
			* power(1.0 - _q, _n - i)
			* payOff(i);
	}
	_optionPrice = _optionPrice / power((1 + _r), _n);
}

BlackScholesLimitBinomialTree::BlackScholesLimitBinomialTree()
{
	_n = 1;
	_maturity = 0.0;
}

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

double BlackScholesLimitBinomialTree::power(double a, int n)
{
	if (n <= 0) {
		return 1;
	}
	return a * power(a, n - 1);
}

int BlackScholesLimitBinomialTree::factorial(int n)
{
	if (n <= 0) {
		return 1;
	}
	return n * factorial(n - 1);
}

double BlackScholesLimitBinomialTree::max(double a, double b)
{
	if (a > b)
		return a;
	return b;
}

double BlackScholesLimitBinomialTree::S_Ni(int i)
{
	if (_u == _d) {
		computeU();
		computeD();
		computeR();
		computeP();
		computeQ();
	}
	return _underlyingPrice * power(_u, i) * power(_d, _n - i);
}

double BlackScholesLimitBinomialTree::payOff(int i)
{
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


