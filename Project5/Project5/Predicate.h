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

	void getParameters(vector<string>& output) {
		for (Parameter p : parameters) {
			output.push_back(p.getParam()); 
		}
	}

	// Places predicate into 1 string 
	string toString() {
		stringstream out; 
		out << name << '(';
		for (long unsigned int i = 0; i < parameters.size(); ++i) {
			out << parameters.at(i).getParam();
			if (i < parameters.size() - 1)
				out << ',';
		}
		out << ')'; 

		return out.str(); 
	}

	// Places predicate into 1 string 
	string toStringCompare() {
		stringstream out;
		out << name; 

		return out.str();
	}



};
