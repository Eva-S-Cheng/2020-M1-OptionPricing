#include "AmericanOptionBinLattice.h"

AmericanOptionBinLattice::AmericanOptionBinLattice()
{
	_n = 1;
	_time = 0.0;
}

AmericanOptionBinLattice::AmericanOptionBinLattice(double time, int n, double volatility, double riskFreeRate, double strike, double underlyingPrice, AmOPType type)
{
	_time = time;
	_n = n;
	_volatility = volatility;
	_riskFreeRate = riskFreeRate;
	_strike = strike;
	_underlyingPrice = underlyingPrice;
	_type = type;
}

AmericanOptionBinLattice::~AmericanOptionBinLattice()
{
	//
}

double AmericanOptionBinLattice::getU()
{
	if (_u == 0)
		computeU();
	return _u;
}

double AmericanOptionBinLattice::getD()
{
	if (_d == 0)
		computeD();
	return _d;
}

double AmericanOptionBinLattice::getP()
{
	if (_p == 0)
		computeP();
	return _p;
}

double AmericanOptionBinLattice::getQ()
{
	if (_q == 0)
		computeQ();
	return _q;
}

double AmericanOptionBinLattice::getDeltaTime()
{
	if (_deltaTime == 0)
		computeDeltaTime();
	return _deltaTime;
}

double AmericanOptionBinLattice::getOptionPrice()
{
	if (_opPrice == 0)
		computePrice();
	return _opPrice;
}

double AmericanOptionBinLattice::getStrike()
{
	return _strike;
}

BinLattice<double> AmericanOptionBinLattice::getPayOff()
{
	return _payOff;
}

bool AmericanOptionBinLattice::getWhetherExercised(int n, int i)
{
	return _exercised.getNode(n, i);
}

void AmericanOptionBinLattice::displayIntrinsicPayOff()
{
	_intrinsicPayOff.stylishDisplay();
}

void AmericanOptionBinLattice::displaypayOff()
{
	_payOff.display();
}

double AmericanOptionBinLattice::power(double a, int n)
{
	if (n <= 0) {
		return 1;
	}
	return a * power(a, n - 1);
}

double AmericanOptionBinLattice::max(double a, double b)
{
	if (a > b)
		return a;
	return b;
}

void AmericanOptionBinLattice::computeU()
{
	if (_deltaTime == 0)
		computeDeltaTime();
	_u = exp(_volatility * sqrt(_deltaTime));
}

void AmericanOptionBinLattice::computeD()
{
	if (_u == 0)
		computeU();
	_d = 1 / _u;
}

void AmericanOptionBinLattice::computeP()
{
	if (_u == _d) {
		computeU();
		computeD();
	}
	if (_u == _d)
		_p = 0;
	else
		_p = (exp(_riskFreeRate * _deltaTime) - _d) / (_u - _d);
}

void AmericanOptionBinLattice::computeQ()
{
	if (_p == 0)
		computeP();
	_q = 1 - _p;
}

void AmericanOptionBinLattice::computeDeltaTime()
{
	_deltaTime = _time / _n;
}

double AmericanOptionBinLattice::S_Ni(int i, int n)
{
	if (_u == 0)
		computeU();
	if (_d == 0)
		computeD();
	return _underlyingPrice * power(_u, i) * power(_d, n - i);
}

void AmericanOptionBinLattice::createIntrinsicPayOff()
{
	_intrinsicPayOff.setN(_n, 0.0);
	for (int i = 0; i <= _n; i++)
	{
		for (int j = 0; j <= i; j++)
		{
			if (_type == AmPut)
				_intrinsicPayOff.setNode(i, j, max(_strike - S_Ni(j, i), 0));
			else 
				_intrinsicPayOff.setNode(i, j, max(S_Ni(j, i) - _strike, 0));
		}
	}

}

void AmericanOptionBinLattice::createPayOff()
{
	createIntrinsicPayOff();
	_payOff.setN(_n, 0.0);
	_exercised.setN(_n, false);
	for (int i = 0; i <= _n; i++)
	{
		_payOff.setNode(_n, i, _intrinsicPayOff.getNode(_n, i));
	}
	for (int i = _n - 1; i <= 0; i--)
	{
		for (int j = 0; j <= i; j++)
		{
			if (_intrinsicPayOff.getNode(i, j) <= (_p * _payOff.getNode(i + 1, j + 1) + _q * _payOff.getNode(i + 1, j)) / (1.0 + _riskFreeRate))
				_exercised.setNode(i, j, true);
			_payOff.setNode(i, j, max(_intrinsicPayOff.getNode(i, j), (_p * _payOff.getNode(i + 1, j + 1) + _q * _payOff.getNode(i + 1, j))/(1.0 + _riskFreeRate)));
		}
	}
}

void AmericanOptionBinLattice::computePrice()
{
	createPayOff();
	_opPrice = _payOff.getNode(0, 0);
}

