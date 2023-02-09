#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include "Predicate.h"
#include "Parameter.h"
#include "Rule.h"
using namespace std;

class DatalogProgram {
private:
	vector<Predicate> schemes;
	vector<Predicate> facts;
	vector<Predicate> queries;
	vector<Rule> rules; 
public:
	DatalogProgram(){}

	void addScheme(Predicate pre) {
		schemes.push_back(pre); 
	}

	void addFact(Predicate pre) {
		facts.push_back(pre); 
	}

	void addQuerie(Predicate pre) {
		queries.push_back(pre); 
	}

	void addRule(Rule rul) {
		rules.push_back(rul);
	}

	string toStringSchemes() {
		stringstream out; 
		out << "Schemes" << '(' << schemes.size() << "):" << endl;
		for (int i = 0; i < schemes.size(); ++i) {
			out << "  " << schemes.at(i).toString() << endl;
		}
		return out.str();
	}

	string toStringFacts() {
		stringstream out;
		out << "Facts" << '(' << facts.size() << "):" << endl;
		for (int i = 0; i < facts.size(); ++i) {
			out << "  " << facts.at(i).toString() << endl;
		}
		return out.str();
	}

	string toStringRules() {
		stringstream out;
		out << "Rules" << '(' << rules.size() << "):" << endl;
		for (int i = 0; i < rules.size(); ++i) {
			out << "  " << rules.at(i).toString() << endl;
		}
		return out.str();
	}

	string toStringQueries() {
		stringstream out;
		out << "Queries" << '(' << queries.size() << "):" << endl;
		for (int i = 0; i < queries.size(); ++i) {
			out << "  " << queries.at(i).toString() << endl;
		}
		return out.str();
	}
	
};