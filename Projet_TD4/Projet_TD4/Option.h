#pragma once
class Option
{
protected:
	/* THE BASIC ATTRIBUTES OR THE OPTIONS */
	int _time;
	int _n;
	double _deltaTime = 0.0;
	double _volatility = 0.0;
	double _riskFreeRate = 0.0;
	double _u = 0.0;
	double _d = 0.0;
	double _p = 0.0;
	double _q = 0.0;
	double _strike = 0.0;
	double _underlyingPrice = 0.0;
	double _optionPrice;

	/* COMPUTERS IN PRIVATE */
	void computeU();
	void computeD();
	void computeP();
	void computeQ();
	void computeDeltaTime();
	void computePrice();

public:
	/* CONSTRUCTOR */
	Option();
	Option(double, int, double, double, double, double);
	~Option();
	/* ABSTRACT METHOD SINCE THE PAYOFF DEPENDS IN THE TYPE */
	virtual double payOff(int) = 0;
	
	/* GETTERS */
	double getU();
	double getD();
	double getP();
	double getQ();
	double getDeltaTime();
	double getOptionPrice();
	double getStrike();

	/* CLASSICAL FUNCTIONS */
	double power(double, int);
	int factorial(int);
	double max(double, double);
	double S_Ni(int);
	
};

/* DERIVATED CLASSES */
class EuropeanPUT : public Option {
public:
	EuropeanPUT();
	EuropeanPUT(double, int, double, double, double, double);
	~EuropeanPUT();

	double payOff(int) override;
};

class EuropeanCALL : public Option {
public:
	EuropeanCALL();
	EuropeanCALL(double, int, double, double, double, double);
	~EuropeanCALL();

	double payOff(int) override;
};

class DigitalPUT : public Option {
public:
	DigitalPUT();
	DigitalPUT(double, int, double, double, double, double);
	~DigitalPUT();

	double payOff(int) override;
};

class DigitalCALL : public Option {
public:
	DigitalCALL();
	DigitalCALL(double, int, double, double, double, double);
	~DigitalCALL();

	double payOff(int) override;
};