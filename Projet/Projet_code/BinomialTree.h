#pragma once
#include "Node.h"


class BinomialTree						// Binomial tree used for the CRR pricer of the TD 5 (creating a real tree)
{
private:
	double _u;							// parameters of the CRR pricer
	double _d;
	int _n;
	double _p;
	double _stockPrice;
	Node* _initial_node;				// It stands for the initial element in the tree (that contains the initial price)
	bool _treeBuilt = false;

public:
	BinomialTree();						// default constructor
	BinomialTree(double, double, int, double, double);
	~BinomialTree();					// destructor
	void buildTree(double, double);		// Creating every nodes from the initial node to the last
	Node* getFirstNode();			
	void display(Node*);				// Display the tree with the informations
};

