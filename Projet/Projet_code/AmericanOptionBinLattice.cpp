#include "AmericanOptionBinLattice.h"

// Default constructor
AmericanOptionBinLattice::AmericanOptionBinLattice()
{
	_n = 1;
}

// Constructor with parameters 
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

// Destructor
AmericanOptionBinLattice::~AmericanOptionBinLattice()
{
	//
}

/* GETTERS */
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

/* DISPLAY THE TREES */
void AmericanOptionBinLattice::displayIntrinsicPayOff()
{
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Displays the intrinsic pay off" << std::endl;
	_intrinsicPayOff.stylishDisplay();
}

void AmericanOptionBinLattice::displaypayOff()
{
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Displays the pay off of an american option" << std::endl;
	_payOff.stylishDisplay();
}

/* MATHEMATICAL FUNCTIONS */
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

// Computing u as always
void AmericanOptionBinLattice::computeU()
{
	if (_deltaTime == 0)
		computeDeltaTime();
	_u = exp(_volatility * sqrt(_deltaTime));
}

// Same for d
void AmericanOptionBinLattice::computeD()
{
	if (_u == 0)
		computeU();
	_d = 1 / _u;
}

// Same for p
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

// Same for q
void AmericanOptionBinLattice::computeQ()
{
	if (_p == 0)
		computeP();
	_q = 1 - _p;
}

void AmericanOptionBinLattice::computeDeltaTime()
{
	_deltaTime = _time/_n;
}

// Computes the price of the option if the price has gone up i times
double AmericanOptionBinLattice::S_Ni(int i, int n)
{
	if (_u == 0)
		computeU();
	if (_d == 0)
		computeD();
	return _underlyingPrice * power(_u, i) * power(_d, n - i);
}

// Creating the tree with the same pay of as the european 
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

// Creates the BinLattice for the payOff
void AmericanOptionBinLattice::createPayOff()
{
	// Creates the payOff 
	createIntrinsicPayOff();
	// Creating the real pay off 
	_payOff.setN(_n, 0.0);
	// creating the boolean bin lattice
	_exercised.setN(_n, false);
	for (int i = 0; i <= _n; i++)
	{
		// The last row is automatially the intrinsic pay off
		_payOff.setNode(_n, i, _intrinsicPayOff.getNode(_n, i));
	}
	for (int i = _n - 1; i >= 0; i--)
	{
		for (int j = 0; j <= i; j++)
		{
			// If the american option is higher than the classical pay off
			if (_intrinsicPayOff.getNode(i, j) <= (_p * _payOff.getNode(i + 1, j + 1) + _q * _payOff.getNode(i + 1, j)) / (1.0 + _riskFreeRate))
				// Should be exercised
				_exercised.setNode(i, j, true);
			// Putting the max value
			_payOff.setNode(i, j, max(_intrinsicPayOff.getNode(i, j), (_p * _payOff.getNode(i + 1, j + 1) + _q * _payOff.getNode(i + 1, j))/(1.0 + _riskFreeRate)));
		}
	}
}

void AmericanOptionBinLattice::displayBool() {
	BinLattice<int> binaryTree;
	binaryTree.setN(_n, 999);


	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Displays 111 if we should use american option, 999 if not" << std::endl;

	for (int i = 0; i <= _n; i++)
	{
		for (int j = 0; j <= i; j++)
		{
			if (_exercised.getNode(i, j) == true)
				binaryTree.setNode(i, j, 111);
		}
	}
	// Displays
	binaryTree.stylishDisplay();
}


void AmericanOptionBinLattice::computePrice()
{
	// Computes the parameters 
	if (_u == 0)
		computeU();
	if (_d == 0)
		computeD();
	if (_p == 0)
		computeP();
	if (_q == 0)
		computeQ();
	if (_deltaTime == 0)
		computeDeltaTime();
	// Creates the payoff
	createPayOff();
	// Computes the price
	_opPrice = _payOff.getNode(0, 0);
}

