#pragma once
#include <iostream>
#include <vector>
#include <sstream>
using namespace std; 

class Scheme : public vector<string> {

public:
	Scheme(){}
	Scheme(vector<string> names) : vector<string>(names) { }

};