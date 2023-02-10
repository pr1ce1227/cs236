#pragma once
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

class Parameter {
private:
	string  param; 

public: 
	Parameter(string value) { param = value;  }

	string getParam() {
		return param; 
	}

	bool operator<(Parameter& par) {
		if (this->param < par.getParam()) {
			return true;
		}
		else
			return false; 
	}

};




