#pragma once
#include <iostream>
#include <vector>
#include "Parameter.h"
#include <string>
#include <sstream>
using namespace std; 

class Predicate {
private: 
	string name;
	vector<Parameter> parameters;

public: 
	Predicate(string name) {this->name = name;}
	Predicate(){}

	void addParam(Parameter par) {
		parameters.push_back(par);
	}

	string getName() {
		return name; 
	}

	string toString() {
		stringstream out; 
		out << name << '(';
		for (int i = 0; i < parameters.size(); ++i) {
			out << parameters.at(i).getParam();
			if (i < parameters.size() - 1)
				out << ',';
		}
		out << ')'; 

		return out.str(); 
	}



};
