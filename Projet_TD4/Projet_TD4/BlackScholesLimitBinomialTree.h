#pragma once

enum BSOpType {
	BSCall, BSPut
};
class BlackScholesLimitBinomialTree
{
private :

	/* Classical parameters of an option */
	double _u = 0.0;
	double _d = 0.0;
	double _r = 0.0;
	double _p = 0.0;
	double _q = 0.0;
	double _volatility = 0.0;
	double _riskFreeRate = 0.0;
	int _n = 1.0;
	double _deltaTime = 0.0;
	double _maturity;
	double _strike = 0.0;
	double _underlyingPrice = 0.0;
	double _optionPrice = 0.0;
	BSOpType _type;

	/* COMPUTERS */
	void computeU();
	void computeD();
	void computeP();
	void computeQ();
	void computeR();
	void computeDeltaTime();
	void computePrice();

public:
	/* CONSTRUCTORS AND DESTRUCTOR */
	BlackScholesLimitBinomialTree();
	BlackScholesLimitBinomialTree(double, double, double, double, double, int, BSOpType);
	~BlackScholesLimitBinomialTree();

	/* CLASSICALN MATHEMATICAL FUNCTIONS */
	double power(double, int);
	double factorial(int);
	double max(double, double);
	double S_Ni(int);
	double payOff(int);

	/* GETTERS */
	double getU();
	double getD();
	double getP();
	double getQ();
	double getDeltaTime();
	double getOptionPrice();
	double getStrike();

};

