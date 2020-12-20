#pragma once
#include "BinomialTree.h"
#include <vector>

enum option_Type {
	call, put
};

class CoxRossRubinsteinEuropeanOption			// CRR Pricer TD 5
{
private :
	BinomialTree _possibilitiesTree;			// The tree created with the prices

	/* PARAMETERS */
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

	std::vector<double> _terminalNodesValues;	// It stores the possibilities of price at maturity
	std::vector<double> _payOffs;				// Stores the pay off for each possibility
	std::vector<Node*> _nodes;					// All the nodes that are contained in the tree

	/* COMPUTERS IN PRIVATE TO AVOID BAD UTILISATION OF THE PROGRAM */
	void computeU();
	void computeD();
	void computeP();
	void computeQ();
	void computeDeltaTime();
	void computeBNValue();

	void createTree();							// It creates the tree if possibilites

	/* CREATING THE VECTORS */
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
	int getN();
	BinomialTree getTree();					// Returns the tree of possibilities
	std::vector<double> getNodesPrices();
	std::vector<double> getPayOffs();
	std::vector<Node*> getNodes();

	void displayTree();						// Display all the information about the CRR Pricer
};

