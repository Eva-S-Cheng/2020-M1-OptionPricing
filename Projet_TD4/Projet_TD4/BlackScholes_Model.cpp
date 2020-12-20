#include "BlackScholes_Model.h"
#include "MonteCarlo_NormalLaw.h"
#include <iostream>


// Default constructor
BlackScholes_Model::BlackScholes_Model() {
	_strikePrice = 0;
	_expiryDate = 0;
	_underlyingPrice = 0;
	_interestRate = 0;
	_volatility = 0;
}

// Constructor with parameters
BlackScholes_Model::BlackScholes_Model(Type type, double strikePrice, double expiryDate, double underlyingPrice, double interestRate, double volatility, int method) {
	_type = type;
	_strikePrice = strikePrice;
	_expiryDate = expiryDate;
	_underlyingPrice = underlyingPrice;
	_interestRate = interestRate;
	_volatility = volatility; 
	_method = method;
}

BlackScholes_Model::~BlackScholes_Model()
{
	//
}

// Verify that we are allowed to compute the price 
bool BlackScholes_Model::computable() {
	if (_volatility == 0 || _strikePrice == 0 || _expiryDate == 0)
		throw std::string("FATAL ERROR : Cannot divide by zero !!");
	else
		return true;
}

// computing d1 and d2 
void BlackScholes_Model::computeD1D2() {
	try
	{
		// Verify that it is computable 
		computable();

		// Computing d1 and d2 with the formulas of the d1 and the d2
		double d1 = (log(_underlyingPrice / _strikePrice) + (_interestRate + _volatility * _volatility / 2.0) * _expiryDate) / (_volatility * sqrt(_expiryDate));
		_d1 = d1;
		_d2 = d1 - _volatility * sqrt(_expiryDate);
	}
	catch (std::string const& error)
	{ 
		std::cerr << error << std::endl;
	}
}

// Computing the delta 
void BlackScholes_Model::computeDelta() {
	double delta;
	// Switch on how the user wants to compute the cdf (Monte Carlo or by using the error function)
	switch (_method)
	{
	case 1:
		delta = calculate_cdf(_d1);
		break;
	case 2:
		delta = calculate_cdf_erf(_d1);
	}

	// The value of delta depends on the type of option
	switch (_type)
	{
	case Call:
		_delta = delta;
		break;
	case Put:
		_delta = delta - 1;
		break;
	default:
		break;
	}
}

// Calculating cdf with monte carlo approach 
double BlackScholes_Model::calculate_cdf(double upperLimit) {
	double cdfValue;
	if (upperLimit < 0) {
		// It is symetric so we can take in consideration only the half of the integral the other part is + 0.5
		// 0 to the absolute value of the upper bound 
		MonteCarlo_NormalLaw MonteCarloComputer(0.0, -upperLimit);
		// Using 800000 points for monte carlo
		MonteCarloComputer.compute(800000);
		// from 0 to a is like from - a to 0 and we add 0.5 to not consider the infinity part
		cdfValue = 0.5 - MonteCarloComputer();
	}
	else {
		// Same but with a positive upper bound 
		MonteCarlo_NormalLaw MonteCarloComputer = MonteCarlo_NormalLaw(0.0, upperLimit);
		MonteCarloComputer.compute(8000000);
		cdfValue = 0.5 + MonteCarloComputer();
	}
	return cdfValue;
}

// Computing the integral with the error function
double BlackScholes_Model::calculate_cdf_erf(double upperLimit) {
	return 0.5 * (1.0 + erf(upperLimit / sqrt(2.0)));
}

void BlackScholes_Model::computePrice() {
	if (_d1 == _d2) {
		computeD1D2();
		computeDelta();
	}

	// Depnding on the user choice (Monte Carlo or error function) and PUT or call
	switch (_type)
	{
		double price;
	case Call:
		switch (_method)
		{
		case 1:
			price = _underlyingPrice * _delta - _strikePrice / exp(_interestRate * _expiryDate) * calculate_cdf(_d2);
			break;
		case 2:
			price = _underlyingPrice * _delta - _strikePrice / exp(_interestRate * _expiryDate) * calculate_cdf_erf(_d2);
			break;
		default:
			break;
		}
		_price = price;
		break;
	case Put:
		switch(_method)
		{
		case 1:
			price = _underlyingPrice * (_delta + 1) - _strikePrice / exp(_interestRate * _expiryDate) * calculate_cdf(_d2);
			break;
		case 2:
			price = _underlyingPrice * (_delta + 1) - _strikePrice / exp(_interestRate * _expiryDate) * calculate_cdf_erf(_d2);
		default:
			break;
		}
		_price = price + _strikePrice/exp(_interestRate * _expiryDate) - _underlyingPrice;
		break;
	default:
		break;
	}
}


/* GETTERS */
double BlackScholes_Model::getDelta() {
	if (_delta == 0.0)
		computeDelta();
	return _delta;
}

double BlackScholes_Model::getD1() {
	if (_d1 == 0.0)
		computeD1D2();
	return _d1;
}

double BlackScholes_Model::getD2() {
	if (_d2== 0.0)
		computeD1D2();
	return _d2;
}

double BlackScholes_Model::getPrice() {
	if (_price == 0.0)
		computePrice();
	return _price;
}

double BlackScholes_Model::getStrikePrice() {
	return _strikePrice;
}

double BlackScholes_Model::getExpiryDate() {
	return  _expiryDate;
}

double BlackScholes_Model::getInterestRate() {
	return _interestRate;
}
double BlackScholes_Model::getVolatility() {
	return _volatility;
}

double BlackScholes_Model::getUnderlyingPrice() {
	return _underlyingPrice;
}

const char* BlackScholes_Model::getType() {
	if (_type == Call)
		return "Call";
	return "Put";
}