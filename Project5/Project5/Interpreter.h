#pragma once
#include <iostream>
#include "DatalogProgram.h"
#include "Database.h"
#include "Relation.h"
#include "Rule.h"
#include <sstream>
#include "Graph.h"
#include "Node.h"
#include <map>
using namespace std; 

class Interpreter {
private:
	vector<Predicate> queries; 
	vector<Relation> solvedQueries; 
	vector<Rule> rules; 

public: 

	vector<Rule> getRules() {
		return rules; 
	}

	static Graph makeGraph(const vector<Rule>& rules) {
		

		Graph graph(rules.size());
		for (unsigned fromID = 0; fromID < rules.size(); fromID++) {
			Rule fromRule = rules.at(fromID);
			//cout << "from rule R" << fromID << ": " << fromRule.toString() << endl;

			for (unsigned pred = 0; pred < fromRule.size(); pred++) {
				Predicate bodyPred = fromRule.getBodyPredicate(pred);
				//cout << "from body predicate: " << bodyPred.toString() << endl;

				for (unsigned toID = 0; toID < rules.size(); toID++) {
					Rule toRule = rules.at(toID);
					//cout << "to rule R" << toID << ": " << toRule.toString() << endl;

					if (bodyPred.toStringCompare() == toRule.getHead().toStringCompare()) {
						//cout << "dependency found: (R" << fromID << ",R" << toID << ")" << endl;
						graph.addEdge(fromID, toID); 
					}
				}
			}
		}
		
		return graph;

	}



	// Takes a datalog program and puts the data into a database 
	void UpdateDatabase(DatalogProgram dp, Database& db) {
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
		
		// Import all the rules 
		rules = dp.rules; 

		// Import all the querires store in a vector 
		queries = dp.queries; 
	}


	// Evaluates a single query 
	Relation evaluateQuery(vector<string> query, string name, Database db) {
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
				rel = rel.select1(i, query.at(i)); 
			}

			// input variable 
			else {
				if (variables.find(query.at(i)) != variables.end()) {
					rel = rel.select2(i, to_string(variables.at(query.at(i))));
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
	void evaluateAllQueries(Database db) {
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
			solvedQueries.push_back(evaluateQuery(query, name, db)); 
		}
	}


	// Evaluates all the rules 
	void evaluateRules(Database& db) {



	
		// Loop through all the rules 
		for (long unsigned int i = 0; i < rules.size(); ++i) {

			// Output current rule 
			cout << rules.at(i).toString() << '.' << endl;

			// Grab the right side rule predicates and create relations vector
			vector<Predicate> body = rules.at(i).getBodyPredicate();
			vector<Relation> relations; 
			vector<vector<string>> names; 

			// Add all body relations to relations vector from database map of relations, for current rule  
			for (long unsigned int j = 0; j < body.size(); ++j) {
				vector<string> input;
				body.at(j).getParameters(input);
				Scheme sch(input);
				Relation rel = db.relations.at(body.at(j).getName()); 
				rel = rel.rename(rel, sch); 
				relations.push_back(rel); 
				names.push_back(input); 
			}

			////////////////////////
			// Selcet functions ////
			////////////////////////

			for (long unsigned int k = 0; k < body.size(); ++k) {

				// Store variables inside of query 
				map<string, int> variables2;
				vector<string> vars_order2;
				// check for constant 
				for (long unsigned int l = 0; l < names.at(k).size(); ++l) {
					if (names.at(k).at(l).at(0) == '\'') {
						relations.at(k) = relations.at(k).select1(l, names.at(k).at(l));
					}

					// input variable 
					else {
						if (variables2.find(names.at(k).at(l)) != variables2.end()) {
							relations.at(k) = relations.at(k).select2(l, to_string(variables2.at(names.at(k).at(l))));
						}
						else {
							variables2.insert({ names.at(k).at(l), l });
							vars_order2.push_back(names.at(k).at(l));
						}
					}
				}
			}

			
			// Will be the final result of joined body relations, start by setting equal to first relation 
			Relation result;
			result = relations.at(0); 
			vector<string> resultVars; 

			// check for more relations then begin to join
			if (relations.size() > 1){
				// Join all the relations together starting with second and stoping one before last 
				for (long unsigned int j = 0; j < relations.size() - 1; ++j) {
					result = result.join(relations.at(j + 1)); 
				}
			}

			// Grab the head predicate name and parameters 
			string headName = rules.at(i).getHead().getName(); 
			vector<string> head_vars;
			rules.at(i).getHead().getParameters(head_vars); 
			map<string, int> variables;
			vector<string> resultScheme = result.getScheme();

			// input all the variables 
			for (long unsigned int j = 0; j < head_vars.size(); ++j) {
				for (long unsigned int k = 0; k < resultScheme.size(); ++k) {
					if (resultScheme.at(k) == head_vars.at(j))
						variables.insert({ head_vars.at(j),k }); 
				}
			}
			

			result = result.project(variables, head_vars); 

			vector<string> vars_order = db.relations.at(headName).getScheme(); 
			result = result.rename(result, vars_order); 

			db.relations.at(headName).unite(result); 

		}
	}

	void evaluateSpecificRules(vector<Rule> rules, Database& db) {




		// Loop through all the rules 
		for (long unsigned int i = 0; i < rules.size(); ++i) {

			// Output current rule 
			cout << rules.at(i).toString() << '.' << endl;

			// Grab the right side rule predicates and create relations vector
			vector<Predicate> body = rules.at(i).getBodyPredicate();
			vector<Relation> relations;
			vector<vector<string>> names;

			// Add all body relations to relations vector from database map of relations, for current rule  
			for (long unsigned int j = 0; j < body.size(); ++j) {
				vector<string> input;
				body.at(j).getParameters(input);
				Scheme sch(input);
				Relation rel = db.relations.at(body.at(j).getName());
				rel = rel.rename(rel, sch);
				relations.push_back(rel);
				names.push_back(input);
			}

			////////////////////////
			// Selcet functions ////
			////////////////////////

			for (long unsigned int k = 0; k < body.size(); ++k) {

				// Store variables inside of query 
				map<string, int> variables2;
				vector<string> vars_order2;
				// check for constant 
				for (long unsigned int l = 0; l < names.at(k).size(); ++l) {
					if (names.at(k).at(l).at(0) == '\'') {
						relations.at(k) = relations.at(k).select1(l, names.at(k).at(l));
					}

					// input variable 
					else {
						if (variables2.find(names.at(k).at(l)) != variables2.end()) {
							relations.at(k) = relations.at(k).select2(l, to_string(variables2.at(names.at(k).at(l))));
						}
						else {
							variables2.insert({ names.at(k).at(l), l });
							vars_order2.push_back(names.at(k).at(l));
						}
					}
				}
			}


			// Will be the final result of joined body relations, start by setting equal to first relation 
			Relation result;
			result = relations.at(0);
			vector<string> resultVars;

			// check for more relations then begin to join
			if (relations.size() > 1) {
				// Join all the relations together starting with second and stoping one before last 
				for (long unsigned int j = 0; j < relations.size() - 1; ++j) {
					result = result.join(relations.at(j + 1));
				}
			}

			// Grab the head predicate name and parameters 
			string headName = rules.at(i).getHead().getName();
			vector<string> head_vars;
			rules.at(i).getHead().getParameters(head_vars);
			map<string, int> variables;
			vector<string> resultScheme = result.getScheme();

			// input all the variables 
			for (long unsigned int j = 0; j < head_vars.size(); ++j) {
				for (long unsigned int k = 0; k < resultScheme.size(); ++k) {
					if (resultScheme.at(k) == head_vars.at(j))
						variables.insert({ head_vars.at(j),k });
				}
			}


			result = result.project(variables, head_vars);

			vector<string> vars_order = db.relations.at(headName).getScheme();
			result = result.rename(result, vars_order);

			db.relations.at(headName).unite(result);

		}
	}


	string toString() {
		stringstream out; 
		/*for (int i = 0; i < solvedQueries.size(); ++i) {
			out << solvedQueries.at(i).toString() << endl;
		}*/
		return out.str(); 
	}


};
