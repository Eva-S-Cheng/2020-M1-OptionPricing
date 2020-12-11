#include "mt19937_singleton.h"
#include <stdio.h>
#include <iostream>

/*
* CODE
* PROVIDED
* BY EVA CHENG
*/

/* ONE CONSTRUCTOR */
mt19937_singleton::mt19937_singleton()
{
	std::random_device device;														// Allowing random
	std::mt19937 generator(device());
	mt19937_singleton_gen = generator;
}

mt19937_singleton& mt19937_singleton::getInstance()									// Returns a reference, a pointer on mt19937_singleton
{
	static std::unique_ptr<mt19937_singleton> instance(new mt19937_singleton());	// Create the unique instance of the class (unique pointer)
	return *instance;																// Returns the pointer on the instance, the reference of the instance
}

std::mt19937 mt19937_singleton::getMT19937_gen()
{
	return mt19937_singleton_gen;
}
