#pragma once
#include "BinLattice.h"
enum AmOPType {
	AmPut, AmCall
};
class AmericanOptionBinLattice
{
private :
	/* CLASSICAL PARAMETERS OF AN OPTION */
	double _time;
	int _n;
	double _deltaTime = 0.0;
	double _volatility = 0.0;
	double _riskFreeRate = 0.0;
	double _u = 0.0;
	double _d = 0.0;
	double _p = 0.0;
	double _q = 0.0;
	double _strike = 0.0;
	double _opPrice = 0.0;
	double _underlyingPrice = 0.0;
	AmOPType _type;
	// Tree which contains the intrisic payOff
	BinLattice<double> _intrinsicPayOff;
	// Tree that contains the real pay off
	BinLattice<double> _payOff;
	// Tree that tells whether the american option should be exercised
	BinLattice<bool> _exercised;

	/* COMPUTERS */
	void computeU();
	void computeD();
	void computeP();
	void computeQ();
	void computeDeltaTime();
	double S_Ni(int, int);
	void computePrice();

	/* TREE CREATORS */
	void createIntrinsicPayOff();
	void createPayOff();

public :
	/* CONSTRUCTORS AND DESTRUCTORS */
	AmericanOptionBinLattice();
	AmericanOptionBinLattice(double, int, double, double, double, double, AmOPType);
	~AmericanOptionBinLattice();

	/* GETTERS */
	double getU();
	double getD();
	double getP();
	double getQ();
	double getDeltaTime();
	double getOptionPrice();
	double getStrike();
	BinLattice<double> getPayOff();
	bool getWhetherExercised(int, int);

	/* DISPLAYS */
	void displayIntrinsicPayOff();
	void displaypayOff();
	void displayBool();

	/* CLASSICAL MATHEMATICAL FUCNTIONS */
	double power(double, int);
	double max(double, double);

};

