#include "Node.h"
#include <iostream>
Node::Node() {
	_address = new int;
	_price = 0.0;
	_p_child = 0.5;
	_q_child = 0.5;
	_level = 1;
}

Node::Node(int level, double probabilityYes, double probabilityNo, double price) {
	_address = new int;
	_price = price;
	_p_child = probabilityYes;
	_q_child = probabilityNo;
	_level = level;
}

Node::~Node()
{
}

double Node::getPrice() {
	if (this == nullptr)
		return 0.0;
	return _price;
}


double Node::getPChild() {
	return _p_child;
}

double Node::getQChild() {
	return _q_child;
}

int* Node::getAddress() {
	if (this == nullptr)
		return 0;
	return _address;
}

void Node::setParentUp(Node* parentUp) {
	_parentUp = parentUp;
}

void Node::setParentDown(Node* parentDown) {
	_parentDown = parentDown;
}

void Node::setChildUp(Node* childUp) {
	_childUp = childUp;
}

void Node::setChildDown(Node* childDown) {
	_childDown = childDown;
}

void Node::setPrice(double price) {
	_price = price;
}

void Node::setPayOff(double r, double deltaT) {
	_payOff = (_p_child * _childUp->getPayOff() + _q_child * _childDown->getPayOff())*exp(-r*deltaT);
}

void Node::setPayOff(double binomialValue) {
	_payOff = binomialValue;
}

Node* Node::getParentUp() {
	if (this == nullptr)
		return nullptr;
	return _parentUp;
}

Node* Node::getParentDown() {
	if (this == nullptr)
		return nullptr;
	return _parentDown;
}

Node* Node::getChildUp() {
	if (this == nullptr)
		return nullptr;
	// std::cout << "not void";
	return _childUp;
}

Node* Node::getChildDown() {
	if (this == nullptr)
		return nullptr;
	return _childDown;
}

int Node::getLevel() {
	return _level;
}

double Node::getPayOff() {
	if (this == nullptr)
		return 0;
	return _payOff;
}
 