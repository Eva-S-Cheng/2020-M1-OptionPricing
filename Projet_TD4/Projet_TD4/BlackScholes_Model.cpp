#include "BlackScholes_Model.h"
#include "MonteCarlo_NormalLaw.h"
#include <iostream>

BlackScholes_Model::BlackScholes_Model() {
	_strikePrice = 0;
	_expiryDate = 0;
	_underlyingPrice = 0;
	_interestRate = 0;
	_volatility = 0;
}

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

bool BlackScholes_Model::computable() {
	if (_volatility == 0 || _strikePrice == 0 || _expiryDate == 0)
		throw std::string("FATAL ERROR : Cannot divide by zero !!");
	else
		return true;
}

void BlackScholes_Model::computeD1D2() {
	try
	{
		computable();
		double d1 = (log(_underlyingPrice / _strikePrice) + (_interestRate + _volatility * _volatility / 2.0) * _expiryDate) / (_volatility * sqrt(_expiryDate));
		_d1 = d1;
		_d2 = d1 - _volatility * sqrt(_expiryDate);
	}
	catch (std::string const& error)
	{ 
		std::cerr << error << std::endl;
	}
}

void BlackScholes_Model::computeDelta() {
	double delta;
	switch (_method)
	{
	case 1:
		delta = calculate_cdf(_d1);
		break;
	case 2:
		delta = calculate_cdf_erf(_d1);
	}
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

double BlackScholes_Model::calculate_cdf(double upperLimit) {
	double cdfValue;
	if (upperLimit < 0) {
		MonteCarlo_NormalLaw MonteCarloComputer(0.0, -upperLimit);
		MonteCarloComputer.compute(800000);
		cdfValue = 0.5 - MonteCarloComputer();
	}
	else {
		MonteCarlo_NormalLaw MonteCarloComputer = MonteCarlo_NormalLaw(0.0, upperLimit);
		MonteCarloComputer.compute(8000000);
		cdfValue = 0.5 + MonteCarloComputer();
	}
	return cdfValue;
}

double BlackScholes_Model::calculate_cdf_erf(double upperLimit) {
	return 0.5 * (1.0 + erf(upperLimit / sqrt(2.0)));
}

void BlackScholes_Model::computePrice() {
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