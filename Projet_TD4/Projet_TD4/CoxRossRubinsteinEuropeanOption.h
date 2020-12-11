#pragma once
#include "BinomialTree.h"
#include <vector>

enum option_Type {
	call, put
};

class CoxRossRubinsteinEuropeanOption
{
private :
	BinomialTree _possibilitiesTree;
	int _time;
	int _n;
	double _deltaTime = 0.0;
	double _volatility = 0.0;
	double _riskFreeRate = 0.0;
	double _u = 0.0;
	double _d = 0.0;
	double _p = 0.0;
	double _q = 0.0;
	double _strike = 0.0;
	double _underlyingPrice = 0.0;

	option_Type _optionType;

	std::vector<double> _terminalNodesValues;
	std::vector<double> _payOffs;
	std::vector<Node*> _nodes;

	void computeU();
	void computeD();
	void computeP();
	void computeQ();
	void computeDeltaTime();
	void computeBNValue();

	void createTree();

	void createTerminalNodesValues();
	void createPayOffs();
	void createNodesVector();


public:
	CoxRossRubinsteinEuropeanOption();
	CoxRossRubinsteinEuropeanOption(double, int, double, double, double, double, option_Type);
	~CoxRossRubinsteinEuropeanOption();

	double getU();
	double getD();
	double getP();
	double getQ();
	double getDeltaTime();
	BinomialTree getTree();
	std::vector<double> getNodesPrices();
	std::vector<double> getPayOffs();
	std::vector<Node*> getNodes();

	void displayTree();
};

