#pragma once
#include <iostream>
#include <vector>
#include "Scheme.h"
#include <sstream>
using namespace std; 

class Tuple : public vector<string> {

public:

	Tuple(vector<string> values) : vector<string>(values) { }

    string toString(const Scheme& scheme) const {
        const Tuple& tuple = *this;
        stringstream out;
        for (long unsigned int i = 0; i < scheme.size(); ++i) {
            out << scheme.at(i) << "=" << tuple.at(i); 
            if (i == scheme.size() - 1) {
                //out << endl;
            }
            else
                out << ", "; 
        }
        return out.str();
    }

};