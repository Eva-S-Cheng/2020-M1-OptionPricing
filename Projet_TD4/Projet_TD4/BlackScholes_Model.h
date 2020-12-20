#pragma once

enum Type {
	Call, Put
};


class BlackScholes_Model
{
private:
	Type _type;
	double _strikePrice;
	double _expiryDate;

	double _underlyingPrice;
	double _interestRate;

	double _volatility;

	double _price = 0;
	double _d1 = 0;
	double _d2 = 0;
	double _delta = 0;
	int _method = 0;

	bool computable();					// Used only internally
	void computeD1D2();					// The user cannot use it and access to it, it allows us to avoid some errors due to the bad usage of the program
	void computeDelta();
	void computePrice();

public:
	BlackScholes_Model();				// default constructor
	BlackScholes_Model(Type type, double strikePrice, double expiryDate, double underlyingPrice, double interestRate, double volatility, int method);
	~BlackScholes_Model();


	/* THE FOLLOWING METHODS ARE THE GETTERS */
	double getD1();
	double getD2();
	double getDelta();
	double getPrice();
	double getStrikePrice();
	double getExpiryDate();
	double getInterestRate();
	double getVolatility();
	double getUnderlyingPrice();
	const char* getType();

	/* TWO METHODS USED TO CALCULATE THE CUMULATIVE DISTRIBUTION FUNCTION */
	double calculate_cdf(double);		// Monte Carlo Approach 
	double calculate_cdf_erf(double);	// Using the error function
};

