#pragma once
#include "Node.h"


class BinomialTree
{
private:
	double _u;
	double _d;
	int _n;
	double _p;
	double _stockPrice;
	Node* _initial_node;
	bool _treeBuilt = false;

public:
	BinomialTree();
	BinomialTree(double, double, int, double, double);
	~BinomialTree();
	void buildTree(double, double);
	Node* getFirstNode();
	void display(Node*);
};

