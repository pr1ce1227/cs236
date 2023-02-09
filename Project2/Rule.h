#pragma once
#include <iostream>
#include <vector>
#include "Predicate.h"
#include <string>
#include <sstream>
using namespace std;

class Rule {
private: 
	Predicate headPredicate; 
	vector<Predicate> bodyPredicate; 

public: 

	Rule(){}

	void addBodyPredicate(Predicate body_) {
		bodyPredicate.push_back(body_); 
	}

	void setHead(Predicate head) {
		headPredicate = head; 
	}

	Predicate getHead() {
		return headPredicate; 
	}

	vector<Predicate> getBodyPredicate() {
		return bodyPredicate; 
	}

	string toString() {
		stringstream out; 
		out << "  " << headPredicate.toString() << " :- ";
		for (int i = 0; i < bodyPredicate.size(); ++i) {
			out << bodyPredicate.at(i).toString(); 
			if (i < bodyPredicate.size() - 1) {
				out << ','; 
			}
		}
		return out.str();
	}


};