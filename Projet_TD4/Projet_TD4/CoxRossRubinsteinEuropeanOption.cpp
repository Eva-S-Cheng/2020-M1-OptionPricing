#include "CoxRossRubinsteinEuropeanOption.h"
#include <iostream>
#include <string>
CoxRossRubinsteinEuropeanOption::CoxRossRubinsteinEuropeanOption() {
	_time = 0;
	_n;
}

CoxRossRubinsteinEuropeanOption::CoxRossRubinsteinEuropeanOption(double time, int n, double volatility,
	double riskFreeRate, double strike, double underlyingPrice, option_Type optionType) {
	_time = time;
	_n = n;
	_volatility = volatility;
	_riskFreeRate = riskFreeRate;
	_strike = strike;
	_underlyingPrice = underlyingPrice;
	_optionType = optionType;
	createTree();
}

CoxRossRubinsteinEuropeanOption::~CoxRossRubinsteinEuropeanOption()
{
	//
}

void CoxRossRubinsteinEuropeanOption::computeDeltaTime() {
	_deltaTime = (double)_time / (double)_n;
}

void CoxRossRubinsteinEuropeanOption::computeU() {
	if (_deltaTime == 0)
		computeDeltaTime();
	_u = exp(_volatility * sqrt(_deltaTime));
}

void CoxRossRubinsteinEuropeanOption::computeD() {
	if (_u == 0)
		computeU();
	_d = 1 / _u;
}

void CoxRossRubinsteinEuropeanOption::computeP() {
	if (_u == _d) {
		computeU();
		computeD();
	}
	if (_u == _d)
		_p = 0;
	else
		_p = (exp(_riskFreeRate * _deltaTime) - _d) / (_u - _d);
}		// Formula seen in the book "Options, futures and other derivatives assets" of Hull


void CoxRossRubinsteinEuropeanOption::computeQ() {
	if (_p == 0)
		computeP();
	_q = 1.0 - _p;
}

void CoxRossRubinsteinEuropeanOption::createTree() {
	if (_u == 0)
		computeU();
	if (_d == 0)
		computeD();
	if (_p == 0)
		computeP();
	if (_q == 0)
		computeQ();
	if (_n == 0)
	{
		_n = 1;
	}
	_possibilitiesTree = BinomialTree(_u, _d, _n, _underlyingPrice, _p);
	_possibilitiesTree.buildTree(_riskFreeRate, _deltaTime);
	//_possibilitiesTree.display(_possibilitiesTree.getFirstNode());
}

void CoxRossRubinsteinEuropeanOption::createTerminalNodesValues() {
	// std::cout << "U = " << _u << std::endl;
	Node* currentNode = _possibilitiesTree.getFirstNode();
	for (int i = 1; i <= _n; i++)
	{
		currentNode = currentNode->getChildUp();
	}

	bool stop = false;
	while (stop == false)
	{
		// std::cout << currentNode->getAddress() << " " << currentNode->getPrice() << std::endl;

		_terminalNodesValues.push_back(currentNode->getPrice());
		if (currentNode->getParentUp() != nullptr) {
			currentNode = currentNode->getParentUp();
			currentNode = currentNode->getChildDown();
			// std::cout << currentNode->getAddress() << " DOWN " << currentNode->getPrice() << std::endl;
		}
		else
			stop = true;
	}
}

void CoxRossRubinsteinEuropeanOption::createNodesVector() {
	std::vector<Node*> queue;
	queue.push_back(_possibilitiesTree.getFirstNode());
	while (!queue.empty()) {
		Node* current = queue.at(0);
		_nodes.push_back(current);
		if (current->getChildUp() != nullptr) {
			if (std::find(queue.begin(), queue.end(), current->getChildUp()) == queue.end())
				queue.push_back(current->getChildUp());
		}
		if (current->getChildDown() != nullptr) {
			if (std::find(queue.begin(), queue.end(), current->getChildDown()) == queue.end())
				queue.push_back(current->getChildDown());
		}
		queue.erase(queue.begin());
	}
}

void CoxRossRubinsteinEuropeanOption::createPayOffs() {
	if (_terminalNodesValues.empty())
		createTerminalNodesValues();
	if (_optionType == call) {
		for (int i = 0; i < _terminalNodesValues.size(); i++) {
			if (_terminalNodesValues.at(i) - _strike > 0)
				_payOffs.push_back((_terminalNodesValues.at(i) - _strike));
			else
				_payOffs.push_back(0.0);
		}
	}
	else if (_optionType == put) {
		for (int i = 0; i < _terminalNodesValues.size(); i++) {
			if (_strike - _terminalNodesValues.at(i) > 0)
				_payOffs.push_back((_strike - _terminalNodesValues.at(i)));
			else
				_payOffs.push_back(0.0);
		}
	}
}


double CoxRossRubinsteinEuropeanOption::getU() {
	return _u;
}

double CoxRossRubinsteinEuropeanOption::getD() {
	return _d;
}

double CoxRossRubinsteinEuropeanOption::getP() {
	return _p;
}

double CoxRossRubinsteinEuropeanOption::getQ() {
	return _q;
}

double CoxRossRubinsteinEuropeanOption::getDeltaTime() {
	return _deltaTime;
}

BinomialTree CoxRossRubinsteinEuropeanOption::getTree() {
	return _possibilitiesTree;
}

std::vector<double> CoxRossRubinsteinEuropeanOption::getNodesPrices() {
	if (_terminalNodesValues.empty())
		createTerminalNodesValues();
	return _terminalNodesValues;
}

std::vector<double> CoxRossRubinsteinEuropeanOption::getPayOffs() {
	if (_payOffs.empty())
		createPayOffs();
	return _payOffs;
}

std::vector<Node*> CoxRossRubinsteinEuropeanOption::getNodes() {
	if (_nodes.empty())
		createNodesVector();
	return _nodes;
}

void CoxRossRubinsteinEuropeanOption::computeBNValue() {
	if (_payOffs.empty())
		createPayOffs();
	if (_nodes.empty())
		createNodesVector();
	std::vector<double> payOff;
	for (int i = 0; i < _payOffs.size(); i++)
	{
		payOff.push_back(_payOffs.at(i));
	}
	for (int i = 0; i < _nodes.size(); i++)
	{
		if (!payOff.empty()) {
			if (_nodes.at(i)->getLevel() == _n) {
				//std::cout << "PO " << payOff.at(0);
				_nodes.at(i)->setPayOff(payOff.at(0));
				payOff.erase(payOff.begin());
			}
		}
	}
	for (int i = _nodes.size() - 1; i >= 0; i--)
	{
		if (_nodes.at(i)->getLevel() < _n) {
			_nodes.at(i)->setPayOff(_riskFreeRate, _deltaTime);
		}
	}
}

void CoxRossRubinsteinEuropeanOption::displayTree() {
	std::cout << std::endl;
	std::cout << "Time : " << _time*365.5 << " days" << std::endl;
	std::cout << "Steps : " << _n << std::endl;
	std::cout << "Time Scale : " << _deltaTime << std::endl;
	std::cout << "Volatility : " << _volatility*100 << " %" << std::endl;
	std::cout << "Risk-free Rate : " << _riskFreeRate*100 << " %"<< std::endl;
	std::cout << "Strike : " << _strike << " Euros" << std::endl;
	std::cout << "Price : " << _underlyingPrice << " Euros" << std::endl;
	if(_optionType == call)
		std::cout << "Type : CALL" << std::endl;
	else
		std::cout << "Type : PUT" << std::endl;

	std::cout << "U = " << _u;
	std::cout << std::endl;
	std::cout << "D = " << _d;
	std::cout << std::endl;
	std::cout << "P = " << _p;
	std::cout << std::endl;
	std::cout << "Q = " << _q;
	std::cout << std::endl;

	std::cout << std::endl;
	std::cout << "=============== PRICES AND BINOMIAL VALUES IN EUROS ===============" << std::endl;
	std::cout << std::endl;
	if (_nodes.empty())
		createNodesVector();
	computeBNValue();
	for (int i = 0; i <= _n; i++) {
		double averagePrice = 0.0;
		double averageBinomialValue = 0.0;
		std::cout << "=== LEVEL " << i << " - " << i + 1 << " VALUES IN EUROS ===" << std::endl;
		for (int j = 0; j < _nodes.size(); j++) {
			if(_nodes.at(j)->getLevel() == i) {
				std::cout << "| P : " << _nodes.at(j)->getPrice() << " - BN Val. : " << _nodes.at(j)->getPayOff() << " |";
				averageBinomialValue = averageBinomialValue + _nodes.at(j)->getPayOff() / (i + 1);
				averagePrice = averagePrice + _nodes.at(j)->getPrice() / (i + 1);
			}
		}
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << "| Expected price at step " << i << " : " << averagePrice << " - Expected binomial value at step " << i << " : " << averageBinomialValue << " |";
		std::cout << std::endl;
		std::cout << std::endl;
	}

	std::cout << "=============== PAY OFFS ===============" << std::endl;
	std::cout << std::endl;
	std::cout << "There are " << getPayOffs().size() << " different Pay Offs you can get !" << std::endl;
	for (int i = 0; i < _payOffs.size(); i++) {
		std::cout << "| Pay-offs : " << _payOffs.at(i) << " Euros |";
	}

	double sum = 0;
	for (int i = 0; i < getPayOffs().size(); i++) {
		if (i != 0 && i != i < getPayOffs().size() - 1) {
			sum = sum + getPayOffs().at(i) * (getP() + getQ());
		}
		else if (i == 0)
			sum = sum + getPayOffs().at(i) * (getP());
		else
			sum = sum + getPayOffs().at(i) * (getQ());
	}

	sum = sum / (getPayOffs().size()+1);
	std::cout << std::endl;
	std::cout << "Expected average pay off : " << sum << std::endl;
}

