#pragma once
#include <iostream>
#include "DatalogProgram.h"
#include "Database.h"
#include "Relation.h"
#include <sstream>
using namespace std; 

class Interpreter {
private:
	DatalogProgram dp; 
	Database db; 
	vector<Predicate> queries; 
	vector<Relation> solvedQueries; 

public: 

	// Takes a datalog program and puts the data into a database 
	void UpdateDatabase(DatalogProgram dp) {

		// Import all the schemes and create a relation for each 
		for (long unsigned int i = 0; i < dp.schemes.size(); ++i) {
			vector<string> input;
			dp.schemes.at(i).getParameters(input);
			Scheme sch(input);
			Relation rel(dp.schemes.at(i).getName(), sch);
			db.addRelation(rel.getName(), rel); 
		}

		// Import facts as tuples to the apropriate relation and scheme name 
		for (long unsigned int i = 0; i < dp.facts.size(); ++i) {
			if (db.relations.find(dp.facts.at(i).getName()) != db.relations.end()) {
				vector<string> input;
				dp.facts.at(i).getParameters(input);
				Tuple tuple(input);
				db.relations.at(dp.facts.at(i).getName()).addTuple(tuple);
			}
		}
		
		// Import all the querires store in a vector 
		queries = dp.queries; 
	}


	// Evaluates a single query 
	Relation evaluateQuery(vector<string> query, string name) {
		// Grab the corresponding relation to the query 
		Relation rel = db.relations.at(name);

		// Store variables inside of query 
		map<string, int> variables;
		vector<string> vars_order; 

		////////////
		// Select //
		////////////

		for (long unsigned int i = 0; i < query.size(); ++i) {
			// check for constant 
			if (query.at(i).at(0) == '\'') {
				rel = rel.select(i, query.at(i)); 
			}

			// input variable 
			else {
				if (variables.find(query.at(i)) != variables.end()) {
					rel = rel.select(i, to_string(variables.at(query.at(i)))); // wrong 
				}
				else {
					variables.insert({ query.at(i), i });
					vars_order.push_back(query.at(i)); 
				}
			}
		}

		// output if tuples were found and how many 
		if (rel.getTuplesSize() != 0) 
			cout << "Yes(" << rel.getTuplesSize() << ')' << endl;
		else
			cout << "No" << endl; 

		// Project //
		rel = rel.project( variables, vars_order); 

		// Rename // 
		rel = rel.rename(rel, vars_order);

		// output 
		cout << rel.toStringTuples();

		
		return rel; 
	}


	// Evaluates all the queries 
	void evaluateAllQueries() {
		// loop through each query, strip out the name and query variables 
		for (long unsigned int i = 0; i < queries.size(); ++i) {
			vector<string> query; 
			string name = queries.at(i).getName(); 
			queries.at(i).getParameters(query);

			// output the query in the correct format 
			cout << queries.at(i).getName() << '('; 
			for (long unsigned int j = 0; j < query.size(); ++j) {
				cout << query.at(j); 
				if (j < query.size() - 1) 
					cout << ',';
				else
					cout << ")? ";
			}

			// solve the actual query 
			solvedQueries.push_back(evaluateQuery(query, name)); 
		}
	}

	string toString() {
		stringstream out; 
		/*for (int i = 0; i < solvedQueries.size(); ++i) {
			out << solvedQueries.at(i).toString() << endl;
		}*/
		return out.str(); 
	}

	Database getDatabase() {
		return db; 
	}

};
