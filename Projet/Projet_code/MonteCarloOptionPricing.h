#pragma once
#include <vector>

enum opType {
	CallOP, PutOP
};
class MonteCarloOptionPricing
{
	// Protected attributes and mehtods that are only accessibles by the derivated classes 
protected:
	// the vector that contains the payOffs 
	std::vector<double> _hT;
	// It is the price of the option
	double _h0 = 0;

	/* CLASSICAL ATTRIBUTES TO CALCULATE THE PRICE OF AN OPTION */
	double _expectedPayOff = 0;
	double _riskFreeRate;
	double _maturity;
	double _strike;
	double _underlyingPrice;
	double _volatility;
	int _numberOfSteps;
	opType _type;

	// Number of occurrencce by Monte Carlo
	int _n;
	// Standard deviation
	double _std = 0.0;
	// Vector that contains the prices 
	std::vector<std::vector<double>> _Si_t;

	// Methods
	void computeH0();
	// Cannot be constructed since it is an abstract class
	MonteCarloOptionPricing();
	// Same
	MonteCarloOptionPricing(double, double, double, double, double, int, int, opType);
	~MonteCarloOptionPricing();
	// Standard deviation to calculate the confidence interval 
	void computeStandardDeviation();

	// virtual methods sicne it depends on the type of option
	virtual void computeExpectedPayOff() = 0;
	virtual void createSi_t() = 0;
	virtual void createHT() = 0;
public:
	// Returns the biggest between 2 numbers 
	double max(double a, double b);
	// Refining the number of iteration
	void refine(int number);
	// Bounds of the confidence interval 
	double uBoundConfInterval();
	double dBouldConfInterval();

	/* GETTES */
	double getH0();
	double getRate();
	double getMaturity();
	double getStrike();
	double getVolatility();
	double getUnderlyingPrice();
	double getNumberOfSteps();
	double getAverage();
	double getSTD();
	double getN();
};

/* DERIVATED CLASSES WITH THE VIRTUAL METHODS IMPLEMENTED */
class EuropeanMonteCarloPricing : public MonteCarloOptionPricing {
private:
	void computeExpectedPayOff();
	void createSi_t();
	void createHT();
public :
	EuropeanMonteCarloPricing();
	EuropeanMonteCarloPricing(double, double, double, double, double, int, opType);
	~EuropeanMonteCarloPricing();
	double WienerProcess(double);
};

class AsianMonteCarloPricing : public MonteCarloOptionPricing {
private :
	void createHT();
	void computeExpectedPayOff();
	void createSi_t();
public:
	AsianMonteCarloPricing();
	AsianMonteCarloPricing(double, double, double, double, double, int, int, opType);
	~AsianMonteCarloPricing();

	
	
};
