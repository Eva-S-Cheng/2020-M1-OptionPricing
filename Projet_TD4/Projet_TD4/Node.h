#pragma once
class Node
{
private:
	int* _address;				// The address of the node (to verify that they are different and they exist
	double _price = 0.0;		// The value of the element 
	double _p_child;			// probability to go to the upchild
	double _q_child;			// probability to go to the down child
	int _level;				

	double _payOff = 0.0;		// binomial value associated to the node

	Node* _childUp;				// address of the up child
	Node* _childDown;
	Node* _parentUp;
	Node* _parentDown;

public:
	Node();
	Node(int, double, double, double = 100);
	~Node();


	/* GETTERS */
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

	/* SETTERS */
	void setParentUp(Node*);
	void setParentDown(Node*);
	void setChildUp(Node*);
	void setChildDown(Node*);

	void setPrice(double);
	void setPayOff(double, double);
	void setPayOff(double);
};

