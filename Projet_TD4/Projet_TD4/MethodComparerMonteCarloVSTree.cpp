#include "MethodComparerMonteCarloVSTree.h"
#include <iostream>

MethodComparerMonteCarloVSTree::MethodComparerMonteCarloVSTree()
{
	//
}

MethodComparerMonteCarloVSTree::MethodComparerMonteCarloVSTree(double riskFreeRate, double maturity, double strike, double underlyingPrice, double volatility, int n, comparerOP type)
{
	_riskFreeRate = riskFreeRate;
	_maturity = maturity;
	_strike = strike;
	_underlyingPrice = underlyingPrice;
	_n = n;
	_type = type;
	_volatility = volatility;

	if (_type == comparerCALL) {
		MCApproach = EuropeanMonteCarloPricing(riskFreeRate, maturity, strike, underlyingPrice, volatility, n, CallOP);
		BSApproach = BlackScholesLimitBinomialTree(riskFreeRate, maturity, strike, underlyingPrice, volatility, n, BSCall);
	}
	else {
		MCApproach = EuropeanMonteCarloPricing(riskFreeRate, maturity, strike, underlyingPrice, volatility, n, PutOP);
		BSApproach = BlackScholesLimitBinomialTree(riskFreeRate, maturity, strike, underlyingPrice, volatility, n, BSPut);
	}
}

void MethodComparerMonteCarloVSTree::displayBest()
{
	std::cout << "Monte Carlo Approach : " << MCApproach.getH0() << std::endl;
	std::cout << "Black Scholes Approach : " << BSApproach.getOptionPrice() << std::endl;

	if (MCApproach.getH0() > BSApproach.getOptionPrice())
		std::cout << "The price by Monte Carlo approach is higher" << std::endl;
	else
		std::cout << "The price by Black Scholes approach is higher" << std::endl;
}

EuropeanMonteCarloPricing MethodComparerMonteCarloVSTree::getMCApproach()
{
	return MCApproach;
}

BlackScholesLimitBinomialTree MethodComparerMonteCarloVSTree::getBSApproach()
{
	return BSApproach;
}
