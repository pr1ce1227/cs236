#pragma once
#include <iostream>
#include <vector>
using namespace std;

class Parameter {

private:
	string param; 

public: 
	Parameter(string value) { param = value;  }

	string getParam() {
		return param; 
	}

};
