#pragma once
class Node
{
private:
	int* _address;
	double _price = 0.0;
	double _p_child;
	double _q_child;
	int _level;

	double _payOff = 0.0;

	Node* _childUp;
	Node* _childDown;
	Node* _parentUp;
	Node* _parentDown;

public:
	Node();
	Node(int, double, double, double = 100);
	~Node();

	double getPrice();
	double getQChild();
	double getPChild();
	int* getAddress();
	double getPayOff();
	Node* getParentUp();
	Node* getParentDown();
	Node* getChildUp();
	Node* getChildDown();
	int getLevel();

	void setParentUp(Node*);
	void setParentDown(Node*);
	void setChildUp(Node*);
	void setChildDown(Node*);

	void setPrice(double);
	void setPayOff(double, double);
	void setPayOff(double);
};

