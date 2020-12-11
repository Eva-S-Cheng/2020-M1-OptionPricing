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

	bool computable();
	void computeD1D2();
	void computeDelta();
	void computePrice();

public:
	BlackScholes_Model();
	BlackScholes_Model(Type type, double strikePrice, double expiryDate, double underlyingPrice, double interestRate, double volatility, int method);
	~BlackScholes_Model();

	double getD1();
	double getD2();
	double getDelta();
	double getPrice();

	const char* getType();
	double getStrikePrice();
	double getExpiryDate();
	double getInterestRate();
	double getVolatility();
	double getUnderlyingPrice();
	double calculate_cdf(double);
	double calculate_cdf_erf(double);
};

