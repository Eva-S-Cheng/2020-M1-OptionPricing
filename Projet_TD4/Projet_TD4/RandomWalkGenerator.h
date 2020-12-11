#pragma once
#include "RandomWalk.h"
#include <vector>
#include <functional>															// For the different distributions

class RandomWalkGenerator
{
private:
	int _size;
	std::vector<RandomWalk> _walks;
	RandomWalkGenerator();

public:
	RandomWalkGenerator(const  RandomWalkGenerator&) = delete;								// Cannot use the default copy constructor
	RandomWalkGenerator& operator = (const RandomWalkGenerator&) = delete;					// Cannot use the default operator assignment

	static RandomWalkGenerator& getInstance();													// Static because the instance is unique (since there is no copy)									
	void generate(int, double, double, std::function<double(double, double)>, Distribution);	// 2 parameter distribution
	RandomWalk operator[](int) const;											// Overloading the brackets operator

	int getSize();
	std::vector<RandomWalk> getWalks();
};

