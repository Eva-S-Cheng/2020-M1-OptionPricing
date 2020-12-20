#pragma once

enum optionName {
	CALL, PUT
};
class CRRPricer_ClosedMethod
{
protected:
	int _n;
	double _time;
	double _riskFreeRate;
	double _deltaTime = 0.0;
	double _volatility = 0.0;
	double _u = 0.0;
	double _d = 0.0;
	double _p = 0.0;
	double _q = 0.0;
	double _strike = 0.0;
	double _underlyingPrice = 0.0;
	double _optionPrice = 0.0;
	optionName _optionType;

	/* COMPUTERS IN PRIVATE TO AVOID BAD UTILISATION OF THE PROGRAM */
	void computeU();
	void computeD();
	void computeP();
	void computeQ();
	void computeDeltaTime();
	void computeOptionPrice();

public:
	CRRPricer_ClosedMethod();
	CRRPricer_ClosedMethod(double, int, double, double, double, double, optionName);

	double getU();
	double getD();
	double getP();
	double getQ();
	double getDeltaTime();
	double getOptionPrice();

	/* MATHEMATIC FUNCTIONS COMMONLY USED TO CALCULATE THE PRICE OF THE OPTION FOR EXAMPLE */
	double power(double, int);
	int factorial(int);
	double max(double, double);
	double S_Ni(int);
};