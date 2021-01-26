#pragma once
#include "MonteCarloOptionPricing.h"
#include "BlackScholesLimitBinomialTree.h"

enum comparerOP {
	comparerCALL, comparerPUT
};

class MethodComparerMonteCarloVSTree
{
private :
	/* 2 approach implemented */
	EuropeanMonteCarloPricing MCApproach;
	BlackScholesLimitBinomialTree BSApproach;

	/* Classical attributes */
	double _volatility = 0.0;
	double _riskFreeRate = 0.0;
	int _n = 1.0;
	double _maturity;
	double _strike = 0.0;
	double _underlyingPrice = 0.0;
	comparerOP _type; 

public:
	MethodComparerMonteCarloVSTree();
	MethodComparerMonteCarloVSTree(double, double, double, double, double, int, comparerOP);
	// Displaying the two prices
	void displayBest();
	// get the approaches
	EuropeanMonteCarloPricing getMCApproach();
	BlackScholesLimitBinomialTree getBSApproach();
};

