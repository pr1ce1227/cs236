#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
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
	set<string> domains; 
public:
	DatalogProgram(){}

	void addDomain(Parameter par) {
		domains.insert(par.getParam()); 
	}

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
		for (long unsigned int i = 0; i < schemes.size(); ++i) {
			out << "  " << schemes.at(i).toString() << endl;
		}
		return out.str();
	}

	string toStringFacts() {
		stringstream out;
		out << "Facts" << '(' << facts.size() << "):" << endl;
		for (long unsigned int i = 0; i < facts.size(); ++i) {
			out << "  " << facts.at(i).toString() << '.' << endl;
		}
		return out.str();
	}

	string toStringRules() {
		stringstream out;
		out << "Rules" << '(' << rules.size() << "):" << endl;
		for (long unsigned int i = 0; i < rules.size(); ++i) {
			out << "  " << rules.at(i).toString() << '.' << endl;
		}
		return out.str();
	}

	string toStringQueries() {
		stringstream out;
		out << "Queries" << '(' << queries.size() << "):" << endl;
		for (long unsigned int i = 0; i < queries.size(); ++i) {
			out << "  " << queries.at(i).toString() << '?' << endl;
		}
		return out.str();
	}

	string toStringDomains() {
		stringstream out;
		out << "Domain" << '(' << domains.size() << "):" << endl;
		set<string>::iterator itr; 
		for (itr = domains.begin(); itr != domains.end(); ++itr) {
			Parameter Par = *itr; 
			out << "  " << Par.getParam() << endl;
		}
		return out.str();
	}
	
};