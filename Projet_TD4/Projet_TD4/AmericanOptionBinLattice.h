#pragma once
#include "BinLattice.h"
enum AmOPType {
	AmPut, AmCall
};
class AmericanOptionBinLattice
{
private :
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
	BinLattice<double> _intrinsicPayOff;
	BinLattice<double> _payOff;
	BinLattice<bool> _exercised;

	void computeU();
	void computeD();
	void computeP();
	void computeQ();
	void computeDeltaTime();
	double S_Ni(int, int);
	void createIntrinsicPayOff();
	void createPayOff();
	void computePrice();
public :
	AmericanOptionBinLattice();
	AmericanOptionBinLattice(double, int, double, double, double, double, AmOPType);
	~AmericanOptionBinLattice();

	double getU();
	double getD();
	double getP();
	double getQ();
	double getDeltaTime();
	double getOptionPrice();
	double getStrike();
	BinLattice<double> getPayOff();
	bool getWhetherExercised(int, int);
	void displayIntrinsicPayOff();
	void displaypayOff();
	void displayBool();

	double power(double, int);
	double max(double, double);

};

