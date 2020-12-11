#pragma once
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <functional>

enum Distribution { uniform, gamma, weibull, extreme_val, norm, lognorm, boxmuller};

// template <class typeOfDistribution>
class RandomWalk
{
private :
	int _size;						// This represents the number of steps
	std::vector<double> _values;	// Contains the value of each walk	
	Distribution _distribution;
	// typeOfDistribution _dist;	// Contain the type of distribution
	double _mean;					// We don't want to compute it every time	

	double computeMean() const;

public:
	RandomWalk();						// Constructor 
	RandomWalk(int, std::vector<double>, Distribution);

	~RandomWalk();						// Destructor

	int getSize() const;				// Returns the size
	double getMean() const;				// Returns the average
	double getVariance() const;			// Returns the variance
	double getSD()	const;				// Returns the standard deviation
	double operator[](int) const;		// Overloading the brackets operator

	void setMean();
	void setMean(double);
};

