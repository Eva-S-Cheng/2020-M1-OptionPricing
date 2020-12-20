#include "RandomWalkGenerator.h"

// Default constructor 
RandomWalkGenerator::RandomWalkGenerator() {
	_size = 0;
}

// Unique generator for the whole running time
RandomWalkGenerator& RandomWalkGenerator::getInstance() {
	static std::unique_ptr<RandomWalkGenerator> unique_instance(new RandomWalkGenerator());
	return *unique_instance;
}

// Generates a random walk following a distribution
void RandomWalkGenerator::generate(int nb_steps, double parameter_a, double parameter_b, std::function<double(double, double)> dist, Distribution d) {
	_size++;
	// Initial value of the distribution
	double init = dist(parameter_a, parameter_b);	// Mean x variance, upper bond x lower bond	
	std::vector<double> value_vector;
	value_vector.push_back(init);
	for (int i = 1; i < nb_steps; i++) {
		// It is a stochastic process that depends in the previous value
		value_vector.push_back(value_vector.at(i - 1) + dist(parameter_a, parameter_b));
	}
	RandomWalk new_element = RandomWalk(nb_steps, value_vector, d);
	_walks.push_back(new_element);
}

// Overloading the brackets 
RandomWalk RandomWalkGenerator::operator[] (int index) const {
	if (index < 0 || index >= _size) {
		std::cout << "Out of range" << std::endl;
		return RandomWalk();
	}
	try
	{
		return _walks.at(index);
	}
	catch (int error)
	{
		std::cout << "Invalid index" << std::endl;
		return RandomWalk();
	}
}

/* GETTERS */
int RandomWalkGenerator::getSize() {
	return _size;
}

std::vector<RandomWalk> RandomWalkGenerator::getWalks() {
	return _walks;
}

