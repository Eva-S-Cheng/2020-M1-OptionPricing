#pragma once
#include <vector>
#include <iostream>
#include <string>

template <class T>
class BinLattice
{
private:
	int _n;
	std::vector<std::vector<T>> _Lattice;
public: 
	void setN(int n, T init)
	{
		_n = n;
		std::vector<T> Lattice;
		for (int i = 0; i <= n; i++)					// n + 1 rows
		{
			for (int j = 0; j <= i; j++) {				// 0 to i, so last iteration 0 to n, n + 1 elements in the last row
				Lattice.push_back(init);				// ex : n = 2
			}											// X
			_Lattice.push_back(Lattice);				// X X
			Lattice.clear();							// X X X
		}
	}
	void setNode(int n, int i, T elementToSet)
	{
		if (_Lattice.size() <= n)
			return;
		if (_Lattice.at(n).size() <= i)
			return;
		_Lattice.at(n).at(i) = elementToSet;
	}
	T getNode(int n, int i) {
		if (_Lattice.size() <= n) {
			std::cout << "Error returning the first element by default" << std::endl;
			n = 0;
		}
		if (_Lattice.at(n).size() <= i) {
			std::cout << "Error returning the first element by default" << std::endl;
			i = 0;
		}
		return _Lattice.at(n).at(i);
	}

	void display() {
		std::cout << std::endl;
		int longest = 0;
		for (int i = 0; i < _Lattice.size(); i++)
		{
			for (int j = 0; j < _Lattice.at(i).size(); j++)
			{
				if (longest < std::to_string(getNode(i, j)).size()) {
					longest = std::to_string(getNode(i, j)).size() + 2;
				}
			}
			std::cout << std::endl;
		}

		for (int i = 0; i < _Lattice.size(); i++)
		{
			for (int j = 0; j < _Lattice.at(i).size(); j++)
			{
				std::string element = std::to_string(getNode(i, j));
				while (element.size() != longest)
				{
					element = element + " ";
				}
				std::cout << element;
			}
			std::cout << std::endl;
		}
	}

	void stylishDisplay() {
		std::cout << std::endl;
		std::cout << std::endl;
		int longest = 0; 
		for (int i = 0; i < _Lattice.size(); i++)
		{
			for (int j = 0; j < _Lattice.at(i).size(); j++)
			{
				if (longest < std::to_string(getNode(i, j)).size()) {
					longest = std::to_string(getNode(i, j)).size() + 1;
				}
			}
		}

		if (longest % 2 == 0) {
			longest = longest + 1;
		}

		std::string templateBetweenRows = " ";									// Meaning / \ or /  \ etc
		int numberOfSpaces = longest - 2;
		int numberToCenter = 0;
		int numberOfSpacesInTheCenter = 0;
		while (numberOfSpaces % 3 == 0) {
			numberOfSpaces = numberOfSpaces - 1;
			numberOfSpacesInTheCenter = numberOfSpacesInTheCenter + 1;
		}
		numberToCenter = numberOfSpaces / 3;
		numberOfSpacesInTheCenter = numberOfSpacesInTheCenter + numberToCenter;
		templateBetweenRows.erase(templateBetweenRows.begin());
		for (int i = 0; i < numberToCenter; i++) {
			templateBetweenRows = templateBetweenRows + ' ';
		}
		templateBetweenRows = templateBetweenRows + '/' + ' ';
		for (int i = 0; i < numberOfSpacesInTheCenter; i++) {
			templateBetweenRows = templateBetweenRows + ' ';
		}
		templateBetweenRows = templateBetweenRows + '\\' + ' ';
		for (int i = 0; i < numberToCenter + 1; i++) {
			templateBetweenRows = templateBetweenRows + ' ';
		}

		int numberOfElementsInTheLastRow = _n + 1;
		int numberOfSpaceBefore = (longest - 1) / 2;
		
		for (int i = 0; i < _Lattice.size() - 1; i++) {						// Except the last row which is particular
			int TimesSpaceNeedToBePrinted = _n - i;
			int NumberOfSpaceToBePrinted = TimesSpaceNeedToBePrinted * numberOfSpaceBefore;
			for (int j = 0; j < NumberOfSpaceToBePrinted; j++) {
				std::cout << ' ';
			}
			for (int j = 0; j <= i; j++) {

				std::string stringToBePrinted = std::to_string(getNode(i, j));
				int lenghtOfTheString = stringToBePrinted.size();
				int spacesBeforeString = longest - lenghtOfTheString;
				if (spacesBeforeString % 2 == 0)
					spacesBeforeString = spacesBeforeString / 2;
				else
					spacesBeforeString = (spacesBeforeString - 1) / 2;
				
				for (int k = 0; k < longest - lenghtOfTheString; k++)
				{
					stringToBePrinted = stringToBePrinted + ' ';
				}
				for (int k = 0; k < spacesBeforeString; k++) {
					std::cout << ' ';
					stringToBePrinted.erase(stringToBePrinted.end());
				}

				std::cout << stringToBePrinted;
			}
			std::cout << std::endl;
			for (int j = 0; j < NumberOfSpaceToBePrinted; j++) {
				std::cout << ' ';
			}

			std::string stringToBePrinted = templateBetweenRows;
			int lenghtOfTheString = stringToBePrinted.size();
			int spacesBeforeString = longest - lenghtOfTheString;
			if (spacesBeforeString % 2 == 0)
				spacesBeforeString = spacesBeforeString / 2;
			else
				spacesBeforeString = (spacesBeforeString - 1) / 2;

			for (int k = 0; k < longest - lenghtOfTheString; k++)
			{
				stringToBePrinted = stringToBePrinted + ' ';
			}
			for (int k = 0; k < spacesBeforeString; k++) {
				std::cout << ' ' ;
				stringToBePrinted.erase(stringToBePrinted.end());
			}

			for (int j = 0; j <= i; j++) {
				std::cout << stringToBePrinted ;
			}
			std::cout << std::endl;
		}
		
		for (int i = 0; i <= _n; i++)
		{
			std::string stringToBePrinted = std::to_string(getNode(_n, i));
			int lenghtOfTheString = stringToBePrinted.size();
			int spacesBeforeString = longest - lenghtOfTheString;
			if (spacesBeforeString % 2 == 0)
				spacesBeforeString = spacesBeforeString / 2;
			else
				spacesBeforeString = (spacesBeforeString - 1) / 2;
			for (int k = 0; k < longest - lenghtOfTheString; k++)
			{
				stringToBePrinted = stringToBePrinted + ' ';
			}
			for (int k = 0; k < spacesBeforeString; k++) {
				std::cout << ' ';
				stringToBePrinted.erase(stringToBePrinted.end());
			}
			std::cout << stringToBePrinted;
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
};


