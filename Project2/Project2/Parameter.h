#pragma once
#include <iostream>
using namespace std;

// Basic datat type for parameters that are just strings 
class Parameter {
private:
	string  param; 

public: 
	Parameter(string value) { param = value;  }

	string getParam() {
		return param; 
	}

};




