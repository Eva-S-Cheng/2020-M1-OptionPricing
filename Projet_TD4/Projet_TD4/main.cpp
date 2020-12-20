#include <stdlib.h>
#include <stdio.h>
#include <functional>
#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <limits>
#include <cstring>


#include "RandomWalk.h"
#include "RandomWalkGenerator.h"
#include "BlackScholes_Model.h"
#include "BinomialTree.h"
#include "CoxRossRubinsteinEuropeanOption.h"
#include "CRRPricer_ClosedMethod.h"
#include "Option.h"
#include "BinLattice.h"

double BoxMullerGenerator(double mean, double variance)			// Centered and reduced variable Z = (X - µ)/sqrt(Var) => X = Z * sqrt(Var) + mean
{																// d = sqrt(-2*ln(U1)/U1) => Z = x * d
	double	x, y, res; 											// X = Z * sqrt(Var) + mean
	if (variance < 0) {
		return NULL;
	}
	else {
		do {
			x = 2.0 * rand() / RAND_MAX - 1;
			y = 2.0 * rand() / RAND_MAX - 1;
			res = x * x + y * y;
		} while (res == 0.0 || res > 1);
		return x * sqrt(-2 * log(res) / res) * sqrt(variance) + mean;
	}
}

double UniformGenerator(double a, double b) {
	std::default_random_engine generator;
	generator.seed(rand());
	std::uniform_real_distribution<double> distribution(a, b);
	return distribution(generator);
}

double GammaGenerator(double a, double b) {
	std::default_random_engine generator;
	generator.seed(rand());
	std::gamma_distribution<double> distribution(a, b);
	return distribution(generator);
}

double WeibullGenerator(double a, double b) {
	std::default_random_engine generator;
	generator.seed(rand());
	std::weibull_distribution<double> distribution(a, b);
	return distribution(generator);
}

double NormalGenerator(double mean, double variance) {
	std::default_random_engine generator;
	generator.seed(rand());
	std::normal_distribution<double> distribution(mean, variance);
	return distribution(generator);
}
	
double LogNormalGenerator(double mean, double variance) {
	std::default_random_engine generator;
	generator.seed(rand());
	std::lognormal_distribution<double> distribution(mean, variance);
	return distribution(generator);
}

double ExtremeValuesGenerator(double a, double b) {
	std::default_random_engine generator;
	generator.seed(rand());
	std::extreme_value_distribution<double> distribution(a, b);
	return distribution(generator);
}

void run_TD4() {
	std::ofstream outData;
	outData.open("B.csv", std::ios::app);
	std::cout << "======== TD 4 : RANDOM WALKS ========" << std::endl << std::endl;
	RandomWalkGenerator& gen = RandomWalkGenerator::getInstance();
	int number_of_walks;
	std::string input;
	do
	{
		std::cout << "How many walks would you like to simulate (integer) ?\n" <<
			"		(Note that you will be able to select the number of steps, the two parameters and the law)" << std::endl;
		std::cin >> input;
		try
		{
			number_of_walks = std::stoi(input);
			if (number_of_walks <= 0)
				std::cout << "Invalid number of walks, try again !" << std::endl;
		}
		catch (std::invalid_argument e)
		{
			std::cout << "Error, invalid argument" << std::endl;
			number_of_walks = -1;
		}
	} while (number_of_walks <= 0);

	for (int i = 0; i < number_of_walks; i++)
	{
		std::cout << "======= RANDOM WALK NUMBER " << (i + 1) << " =======" << std::endl;
		int number_of_steps;
		double first_parameter;
		double second_parameter;
		int choice_of_distribution;
		Distribution dist;
		double(*function_random)(double, double) = UniformGenerator;
		bool correct_parameter;
		do
		{
			std::cout << "How many steps would you like to simulate (integer) ?" << std::endl;
			std::cin >> input;
			try
			{
				number_of_steps = std::stoi(input);
				if (number_of_steps <= 0)
					std::cout << "Invalid number of walks, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				number_of_steps = -1;
			}
		} while (number_of_steps <= 0);

		do
		{
			std::cout << "Enter a double first parameter (lowerbond, mean or...) :" << std::endl;
			std::cin >> input;
			try
			{
				first_parameter = std::stod(input);
				correct_parameter = true;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				correct_parameter = false;
			}
		} while (!correct_parameter);

		do
		{
			std::cout << "Enter a double second parameter (lowerbond, mean or...) :" << std::endl;
			std::cin >> input;
			try
			{
				second_parameter = std::stod(input);
				correct_parameter = true;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				correct_parameter = false;
			}
		} while (!correct_parameter);

		do
		{
			std::cout << "Select a distribution :\n" <<
				"		1. Uniform Distribution\n" <<
				"		2. Gamma Distribution\n"
				"		3. Weibull Distribution\n"
				"		4. Extreme Values Distribution\n"
				"		5. Normal Distribution\n"
				"		6. LogNormal Distribution\n"
				"		7. Box Muller Distribution\n" << std::endl;
			std::cin >> input;
			try
			{
				choice_of_distribution = std::stoi(input);

				switch (choice_of_distribution)
				{
				case 1:
					std::cout << "Uniform Distribution" << std::endl;
					dist = uniform;
					function_random = UniformGenerator;
					break;
				case 2:
					std::cout << "Gamma Distribution" << std::endl;
					dist = gamma;
					function_random = GammaGenerator;
					break;
				case 3:
					std::cout << "Weibull Distribution" << std::endl;
					dist = weibull;
					function_random = WeibullGenerator;
					break;
				case 4:
					std::cout << "Extreme Values Distribution" << std::endl;
					dist = extreme_val;
					function_random = ExtremeValuesGenerator;
					break;
				case 5:
					std::cout << "Normal Distribution" << std::endl;
					dist = norm;
					function_random = NormalGenerator;
					break;
				case 6:
					std::cout << "Log Normal Distribution" << std::endl;
					dist = lognorm;
					function_random = LogNormalGenerator;
					break;
				case 7:
					std::cout << "Box Muller Distribution" << std::endl;
					dist = boxmuller;
					function_random = BoxMullerGenerator;
					break;
				default:
					std::cout << "Default Error : Uniform Distribution" << std::endl;
					dist = uniform;
					function_random = UniformGenerator;
					break;
				}

			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				correct_parameter = false;
			}
		} while (choice_of_distribution <= 0 || choice_of_distribution > 7);
		gen.generate(number_of_steps, first_parameter, second_parameter, function_random, dist);
	}

	for (int i = 0; i < gen.getSize(); i++)
	{
		std::cout << "Walk " << (i + 1) << ";" << gen[i][0];
		outData << "Walk " << (i + 1) << ";" << gen[i][0];
		for (int j = 1; j < gen[i].getSize(); j++)
		{
			std::cout << ";" << gen[i][j];
			outData << ";" << gen[i][j];
		}
		std::cout << std::endl;
		outData << std::endl;
	}
}

void run_TD5_BlackScholes() {
	std::vector<BlackScholes_Model> blackScholesVector;
	int number_of_models;
	std::string input;

	do
	{
		std::cout << "For how many options would you like to simulate the price (integer) ?" << std::endl;
		std::cin >> input;
		try
		{
			number_of_models = std::stoi(input);
			if (number_of_models <= 0)
				std::cout << "Invalid number of models, try again !" << std::endl;
		}
		catch (std::invalid_argument e)
		{
			std::cout << "Error, invalid argument" << std::endl;
			number_of_models = -1;
		}
	} while (number_of_models <= 0);

	for (int i = 0; i < number_of_models; i++) {
		bool valid_input = false;
		int choice = 0;
		Type option;
		do
		{
			std::cout << "Select an option :\n	1. Call\n	2. Put" << std::endl;
			std::cin >> input;
			try
			{
				choice = std::stoi(input);
				if (choice != 1 && choice != 2)
					std::cout << "Invalid choice, try again !" << std::endl;
				else
					valid_input = true;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
			}
		} while (!valid_input);

		switch (choice)
		{
		case 1:
			option = Call;
			break;
		case 2:
			option = Put;
		default:
			break;
		}

		double strikePrice;
		do
		{
			std::cout << "Enter a strike price : " << std::endl;
			std::cin >> input;
			try
			{
				strikePrice = std::stod(input);
				if (strikePrice <=0)
					std::cout << "Invalid price, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				strikePrice = -1;
			}
		} while (strikePrice <= 0);


		double maturity;
		do
		{
			std::cout << "Enter a maturity in years (0.5 for 6 months) : " << std::endl;
			std::cin >> input;
			try
			{
				maturity = std::stod(input);
				if (maturity <= 0 || maturity >= 50)
					std::cout << "Invalid maturity, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				maturity = -1;
			}
		} while (maturity <= 0 || maturity >= 50);

		double underlyingPrice;
		do
		{
			std::cout << "Enter an underlying price : " << std::endl;
			std::cin >> input;
			try
			{
				underlyingPrice = std::stod(input);
				if (underlyingPrice <= 0)
					std::cout << "Invalid price, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				underlyingPrice = -1;
			}
		} while (underlyingPrice <= 0);

		double volatility;
		do
		{
			std::cout << "Enter a volatility in % (max 100) : " << std::endl;
			std::cin >> input;
			try
			{
				volatility = std::stod(input);
				volatility = volatility / 100.0;
				if (volatility <= 0||volatility>= 1)
					std::cout << "Invalid maturity, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				volatility = -1;
			}
		} while (volatility <= 0 || volatility >= 1);

		double riskFreeRate;
		do
		{
			std::cout << "Enter a Rate in % (max 100) : " << std::endl;
			std::cin >> input;
			try
			{
				riskFreeRate = std::stod(input);
				riskFreeRate = riskFreeRate / 100.0;
				if (riskFreeRate <= 0 || riskFreeRate >= 1)
					std::cout << "Invalid maturity, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				riskFreeRate = -1;
			}
		} while (riskFreeRate <= 0 || riskFreeRate >= 1);

		int method;
		do
		{
			std::cout << "Select a method you want to use for the cdf :\n	1. Monte Carlo Modelisation\n	2. Gauss Error Function" << std::endl;
			std::cin >> input;
			try
			{
				method = std::stoi(input);
				if (method != 1 && method != 2)
					std::cout << "Invalid choice, try again !" << std::endl;
				else
					method = true;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				method = 0;
			}
		} while (method != 1 && method != 2);

		blackScholesVector.push_back(BlackScholes_Model(option, strikePrice, maturity, underlyingPrice, riskFreeRate, volatility, method));
	}


	for (int i = 0; i < blackScholesVector.size(); i++) {
		std::cout << "============================================" << std::endl;
		std::cout << "UNDERLYING PRICE : " << blackScholesVector.at(i).getUnderlyingPrice() << std::endl;
		std::cout << "STRIKE PRICE : " << blackScholesVector.at(i).getStrikePrice() << std::endl;
		std::cout << "MATURITY DATE : " << blackScholesVector.at(i).getExpiryDate() * 12 << " months" << std::endl;
		std::cout << "INTEREST RATE : " << blackScholesVector.at(i).getInterestRate() * 100 << "%" << std::endl;
		std::cout << "VOLATILITY : " << blackScholesVector.at(i).getVolatility() * 100 << "%" << std::endl;

		std::cout << std::endl;

		std::cout << "D1 = " << blackScholesVector.at(i).getD1() << std::endl;
		std::cout << "D2 = " << blackScholesVector.at(i).getD2() << std::endl;
		std::cout << "DELTA = " << blackScholesVector.at(i).getDelta() << std::endl;
		std::cout << "OPTION PRICE = " << blackScholesVector.at(i).getPrice() << std::endl;

		std::cout << std::endl;
		std::cout << std::endl;
	}

}

void run_TD5_CRR() {
	std::vector<CoxRossRubinsteinEuropeanOption> CRRVector;
	int number_of_CRR;
	std::string input;

	do
	{
		std::cout << "For how many options would you like to simulate the price (integer) ?" << std::endl;
		std::cin >> input;
		try
		{
			number_of_CRR = std::stoi(input);
			if (number_of_CRR <= 0)
				std::cout << "Invalid number of models, try again !" << std::endl;
		}
		catch (std::invalid_argument e)
		{
			std::cout << "Error, invalid argument" << std::endl;
			number_of_CRR = -1;
		}
	} while (number_of_CRR <= 0);

	for (int i = 0; i < number_of_CRR; i++) {
		bool valid_input = false;
		int choice = 0;
		option_Type option;
		do
		{
			std::cout << "Select an option :\n	1. Call\n	2. Put" << std::endl;
			std::cin >> input;
			try
			{
				choice = std::stoi(input);
				if (choice != 1 && choice != 2)
					std::cout << "Invalid choice, try again !" << std::endl;
				else
					valid_input = true;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
			}
		} while (!valid_input);

		switch (choice)
		{
		case 1:
			option = call;
			break;
		case 2:
			option = put;
		default:
			break;
		}

		double strikePrice;
		do
		{
			std::cout << "Enter a strike price : " << std::endl;
			std::cin >> input;
			try
			{
				strikePrice = std::stod(input);
				if (strikePrice <= 0)
					std::cout << "Invalid price, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				strikePrice = -1;
			}
		} while (strikePrice <= 0);

		double underlyingPrice;
		do
		{
			std::cout << "Enter an underlying price : " << std::endl;
			std::cin >> input;
			try
			{
				underlyingPrice = std::stod(input);
				if (underlyingPrice <= 0)
					std::cout << "Invalid price, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				underlyingPrice = -1;
			}
		} while (underlyingPrice <= 0);

		double volatility;
		do
		{
			std::cout << "Enter a volatility in % (max 100) : " << std::endl;
			std::cin >> input;
			try
			{
				volatility = std::stod(input);
				volatility = volatility / 100.0;
				if (volatility <= 0 || volatility >= 1)
					std::cout << "Invalid volatility, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				volatility = -1;
			}
		} while (volatility <= 0 || volatility >= 1);

		double riskFreeRate;
		do
		{
			std::cout << "Enter a Rate in % (max 100) : " << std::endl;
			std::cin >> input;
			try
			{
				riskFreeRate = std::stod(input);
				riskFreeRate = riskFreeRate / 100.0;
				if (riskFreeRate <= 0 || riskFreeRate >= 1)
					std::cout << "Invalid rate, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				riskFreeRate = -1;
			}
		} while (riskFreeRate <= 0 || riskFreeRate >= 1);

		double time;
		do
		{
			std::cout << "How much time would you like to consider ?" << std::endl;
			std::cin >> input;
			try
			{
				time = std::stod(input);
				if (time <= 0)
					std::cout << "Invalid time, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				time = -1;
			}
		} while (time <= 0);

		int periods;
		do
		{
			std::cout << "How many periods would you like to consider ?" << std::endl;
			std::cin >> input;
			try
			{
				periods = std::stoi(input);
				if (periods <= 0)
					std::cout << "Invalid number of periods, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				periods = -1;
			}
		} while (periods <= 0);

		CRRVector.push_back(CoxRossRubinsteinEuropeanOption(time, periods, volatility, riskFreeRate, strikePrice, underlyingPrice, option));
	}

	for (int i = 0; i < CRRVector.size(); i++) {
		std::cout << std::endl;
		std::cout << std::endl;
		CRRVector.at(i).displayTree();
		std::cout << std::endl;
		std::cout << "Estimated Option Price by Binomial Value : " << CRRVector.at(i).getNodes().at(0)->getPayOff();
		std::cout << std::endl;
		std::cout << std::endl;
		int choice = 0;

		bool valid_input = false;
		do
		{
			std::cout << "Display tree ? (1. Yes | 2. No)" << std::endl;
			std::cin >> input;
			try
			{
				choice = std::stoi(input);
				if (choice != 1 && choice != 2)
					std::cout << "Invalid choice, try again !" << std::endl;
				else
					valid_input = true;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
			}
		} while (!valid_input);

		if (choice == 1) {
			BinLattice<int> test;
			BinLattice<double> test2;
			test.setN(CRRVector.at(i).getN(), 1.0);
			test2.setN(CRRVector.at(i).getN(), 2.0);
			int start = 0;
			for (int j = 0; j <= CRRVector.at(i).getN(); j++)
			{
				for (int k = 0; k <= j; k++) {
					test.setNode(j, k, round(CRRVector.at(j).getNodes().at(start)->getPrice()));
					test2.setNode(j, k, CRRVector.at(j).getNodes().at(start)->getPayOff());
					start++;
				}
			}
			test.display();
			test2.display();
			test.stylishDisplay();
			test2.stylishDisplay();

		}
	}
	
}

void run_TD6_CRR() {
	std::vector<CRRPricer_ClosedMethod> CRRVector;
	int number_of_CRR;
	std::string input;

	do
	{
		std::cout << "For how many options would you like to simulate the price (integer) ?" << std::endl;
		std::cin >> input;
		try
		{
			number_of_CRR = std::stoi(input);
			if (number_of_CRR <= 0)
				std::cout << "Invalid number of models, try again !" << std::endl;
		}
		catch (std::invalid_argument e)
		{
			std::cout << "Error, invalid argument" << std::endl;
			number_of_CRR = -1;
		}
	} while (number_of_CRR <= 0);

	for (int i = 0; i < number_of_CRR; i++) {
		bool valid_input = false;
		int choice = 0;
		optionName option;
		do
		{
			std::cout << "Select an option :\n	1. Call\n	2. Put" << std::endl;
			std::cin >> input;
			try
			{
				choice = std::stoi(input);
				if (choice != 1 && choice != 2)
					std::cout << "Invalid choice, try again !" << std::endl;
				else
					valid_input = true;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
			}
		} while (!valid_input);

		switch (choice)
		{
		case 1:
			option = CALL;
			break;
		case 2:
			option = PUT;
		default:
			break;
		}

		double strikePrice;
		do
		{
			std::cout << "Enter a strike price : " << std::endl;
			std::cin >> input;
			try
			{
				strikePrice = std::stod(input);
				if (strikePrice <= 0)
					std::cout << "Invalid price, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				strikePrice = -1;
			}
		} while (strikePrice <= 0);


		double maturity;
		do
		{
			std::cout << "Enter a maturity in years (0.5 for 6 months) : " << std::endl;
			std::cin >> input;
			try
			{
				maturity = std::stod(input);
				if (maturity <= 0 || maturity >= 50)
					std::cout << "Invalid maturity, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				maturity = -1;
			}
		} while (maturity <= 0 || maturity >= 50);

		double underlyingPrice;
		do
		{
			std::cout << "Enter an underlying price : " << std::endl;
			std::cin >> input;
			try
			{
				underlyingPrice = std::stod(input);
				if (underlyingPrice <= 0)
					std::cout << "Invalid price, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				underlyingPrice = -1;
			}
		} while (underlyingPrice <= 0);

		double volatility;
		do
		{
			std::cout << "Enter a volatility in % (max 100) : " << std::endl;
			std::cin >> input;
			try
			{
				volatility = std::stod(input);
				volatility = volatility / 100.0;
				if (volatility <= 0 || volatility >= 1)
					std::cout << "Invalid volatility, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				volatility = -1;
			}
		} while (volatility <= 0 || volatility >= 1);

		double riskFreeRate;
		do
		{
			std::cout << "Enter a Rate in % (max 100) : " << std::endl;
			std::cin >> input;
			try
			{
				riskFreeRate = std::stod(input);
				riskFreeRate = riskFreeRate / 100.0;
				if (riskFreeRate <= 0 || riskFreeRate >= 1)
					std::cout << "Invalid rate, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				riskFreeRate = -1;
			}
		} while (riskFreeRate <= 0 || riskFreeRate >= 1);


		int periods;
		do
		{
			std::cout << "How many periods would you like to consider ?" << std::endl;
			std::cin >> input;
			try
			{
				periods = std::stoi(input);
				if (periods <= 0)
					std::cout << "Invalid number of periods, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				periods = -1;
			}
		} while (periods <= 0);

		CRRVector.push_back(CRRPricer_ClosedMethod(maturity, periods, volatility, riskFreeRate, strikePrice, underlyingPrice, option));
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << "Time : " << maturity << " years" << std::endl;
		std::cout << "Periods : " << periods << std::endl;
		std::cout << "Volatility : " << volatility * 100 << " %" << std::endl;
		std::cout << "Rate : " << riskFreeRate * 100 << " %" << std::endl;
		std::cout << "Strike : " << volatility << " E" << std::endl;
		std::cout << "Underlying price : " << underlyingPrice << " E" << std::endl;
		std::cout << "type : " << option  << std::endl;
		double op = CRRVector.at(i).getOptionPrice();
		std::cout << "Option Price : " << op << " E" << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;

	}

}


void run_TD6_OptionPayOffs() {
	int number_of_CRR;
	std::string input;
	std::vector<Option*> options;

	do
	{
		std::cout << "For how many options would you like to simulate the price (integer) ?" << std::endl;
		std::cin >> input;
		try
		{
			number_of_CRR = std::stoi(input);
			if (number_of_CRR <= 0)
				std::cout << "Invalid number of models, try again !" << std::endl;
		}
		catch (std::invalid_argument e)
		{
			std::cout << "Error, invalid argument" << std::endl;
			number_of_CRR = -1;
		}
	} while (number_of_CRR <= 0);

	for (int i = 0; i < number_of_CRR; i++) {

		double strikePrice;
		do
		{
			std::cout << "Enter a strike price : " << std::endl;
			std::cin >> input;
			try
			{
				strikePrice = std::stod(input);
				if (strikePrice <= 0)
					std::cout << "Invalid price, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				strikePrice = -1;
			}
		} while (strikePrice <= 0);


		double maturity;
		do
		{
			std::cout << "Enter a maturity in years (0.5 for 6 months) : " << std::endl;
			std::cin >> input;
			try
			{
				maturity = std::stod(input);
				if (maturity <= 0 || maturity >= 50)
					std::cout << "Invalid maturity, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				maturity = -1;
			}
		} while (maturity <= 0 || maturity >= 50);

		double underlyingPrice;
		do
		{
			std::cout << "Enter an underlying price : " << std::endl;
			std::cin >> input;
			try
			{
				underlyingPrice = std::stod(input);
				if (underlyingPrice <= 0)
					std::cout << "Invalid price, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				underlyingPrice = -1;
			}
		} while (underlyingPrice <= 0);

		double volatility;
		do
		{
			std::cout << "Enter a volatility in % (max 100) : " << std::endl;
			std::cin >> input;
			try
			{
				volatility = std::stod(input);
				volatility = volatility / 100.0;
				if (volatility <= 0 || volatility >= 1)
					std::cout << "Invalid volatility, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				volatility = -1;
			}
		} while (volatility <= 0 || volatility >= 1);

		double riskFreeRate;
		do
		{
			std::cout << "Enter a Rate in % (max 100) : " << std::endl;
			std::cin >> input;
			try
			{
				riskFreeRate = std::stod(input);
				riskFreeRate = riskFreeRate / 100.0;
				if (riskFreeRate <= 0 || riskFreeRate >= 1)
					std::cout << "Invalid rate, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				riskFreeRate = -1;
			}
		} while (riskFreeRate <= 0 || riskFreeRate >= 1);


		int periods;
		do
		{
			std::cout << "How many periods would you like to consider ?" << std::endl;
			std::cin >> input;
			try
			{
				periods = std::stoi(input);
				if (periods <= 0)
					std::cout << "Invalid number of periods, try again !" << std::endl;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				periods = -1;
			}
		} while (periods <= 0);

		int choice;
		bool validChoice = false;
		do
		{
			std::cout << "What type of option is it ?\n	1. European Put\n	2. European Call\n	3. Digital Put\n	4. Digital Call" << std::endl;
			std::cin >> input;
			try
			{
				choice = std::stoi(input);
				if (choice < 1 || choice > 4)
					std::cout << "Invalid choice, try again !" << std::endl;
				else
					validChoice = true;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
			}
		} while (!validChoice);

		int ups;
		switch (choice) {
		case 1:
			options.push_back(new EuropeanPUT(maturity, periods, volatility, riskFreeRate, strikePrice, underlyingPrice));
			break;
		case 2:
			options.push_back(new EuropeanCALL(maturity, periods, volatility, riskFreeRate, strikePrice, underlyingPrice));
			break;
		case 3:
			options.push_back(new DigitalPUT(maturity, periods, volatility, riskFreeRate, strikePrice, underlyingPrice));
			break;
		case 4:
			options.push_back(new DigitalCALL(maturity, periods, volatility, riskFreeRate, strikePrice, underlyingPrice));
			break;
		default :
			break;
		}

		std::cout << "Time : " << maturity << " years" << std::endl;
		std::cout << "Periods : " << periods << std::endl;
		std::cout << "Volatility : " << volatility * 100 << " %" << std::endl;
		std::cout << "Rate : " << riskFreeRate * 100 << " %" << std::endl;
		std::cout << "Strike : " << strikePrice << " E" << std::endl;
		std::cout << "Underlying price : " << underlyingPrice << " E" << std::endl;
		std::cout << "Option Price : " << options[i]->getOptionPrice() << " E" << std::endl;

		std::cout << std::endl;
		std::cout << "Enter the number of up to get the expected Pay off :" << std::endl;

		do
		{
			std::cout << "How many UPs would you like to expect ?" << std::endl;
			std::cin >> input;
			try
			{
				ups = std::stoi(input);
				if (ups < 0 || choice > periods)
					std::cout << "Invalid choice, try again !" << std::endl;
				else
					validChoice = true;
			}
			catch (std::invalid_argument e)
			{
				std::cout << "Error, invalid argument" << std::endl;
				ups = -1;
			}
		} while (ups < 0 || ups > periods);
		std::cout << "Expected Pay Off : " << options[i]->payOff(ups);
		std::cout << std::endl;
	}
}

int main() {
	
	return 0;
}