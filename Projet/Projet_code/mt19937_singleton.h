#include <random>
#pragma once

class mt19937_singleton
{
public:
	/* These deleted methods prevent the compilator to use the default method and don't need to be implemented */
	mt19937_singleton(const  mt19937_singleton&) = delete;						// Cannot use the default copy constructor
	mt19937_singleton& operator = (const mt19937_singleton&) = delete;			// Cannot use the default operator assignment

	/* CLASS METHOD */
	static mt19937_singleton& getInstance();									// Static because the instance is unique (since there is no copy)									
	std::mt19937 getMT19937_gen();												// Random numnber generator
private:
	mt19937_singleton();
	std::mt19937 mt19937_singleton_gen;
};

