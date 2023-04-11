#include <iostream>
#include <vector>
#include <sstream>

#include "Node.h"
#include "Graph.h"
#include "Database.h"
#include "DatalogProgram.h"
#include "Interpreter.h"
#include "Rule.h"
#include "Parameter.h"
#include "Parser.h"
#include "Scanner.h"
#include <sstream>
using namespace std; 

int main(int argc, char* argv[]) {

	/////////// 
	// Files // 
	///////////

	// Input file into a string called input
	string fileName = argv[1];
	ifstream in;
	in.open(fileName);
	stringstream ss;
	ss << in.rdbuf();
	string input = ss.str();
	in.close();

	/////////////
	// Scanner //
	/////////////

	// Scan file for tokens 
	Scanner file(input);

	/////////////
	// Parser ///
	/////////////

	// Parse tokens into facts, schemes, rules, queries and domains
	// Throws an error if invalid input or in the wrong order
	Parser program = file.scanToken();
	program.parse();

	/////////////////
	// Interpreter //
	/////////////////

	// input datalog program data into interpreter to be sorted 
	Interpreter ip;
	DatalogProgram dp = program.getDP();
	Database db;

	// fill the database by interpreting the datalog program 
	ip.UpdateDatabase(dp, db);

	// Make dependency graph 
	Graph graph = Interpreter::makeGraph(ip.getRules());
	cout << "Dependency Graph" << endl << graph.toString()  << endl;

	// Reverse the graph 
	Graph rev_graph = graph.reverseGraph(); 

	// Create a depth first forest and store valuse in post order from reversed graph 
	vector<int> postOrder_rev = rev_graph.getPostOrder(); 

	// Solve forward tree using order from reversed tree post order 
	vector<vector<int>> ruleOrder = graph.dfsForward(postOrder_rev); 
	for (long unsigned int i = 0; i < ruleOrder.size(); ++i) {
		sort(ruleOrder.at(i).begin(),ruleOrder.at(i).end()); 
	}

	// Import and evaluate rules 
	cout << "Rule Evaluation" << endl; 
	vector<Rule> rules = ip.getRules();

	// evaluate rules by their SCC in the order decided by ruleOrder
	for (long unsigned int i = 0; i < ruleOrder.size(); ++i) {

		// Grab rules used in current SCC 
		vector<Rule> rules_evaluate;
		cout << "SCC: ";
		for (long unsigned int j = 0; j < ruleOrder.at(i).size(); ++j) {
			cout << 'R' << ruleOrder.at(i).at(j); 
			if (j < ruleOrder.at(i).size() - 1) {
				cout << ','; 
			}
			rules_evaluate.push_back(rules.at(ruleOrder.at(i).at(j))); 
		}
		cout << endl; 

		// Counters to see if rules need to be calculate again 
		int oldCount = 0;
		int newCount = 0;
		int counter = 0;

		// If only one rule that doesn't reference itself only solve once, else solve like normal  
		if (rules_evaluate.size() == 1  && rules_evaluate.at(0).getBodyPredicate().at(0).getName() != rules_evaluate.at(0).getHead().getName()) {

			// if one rule that has more than one body predicate, check to see if second references head rule if so evaluate once else proceed 
			//  like normal 
			if (rules_evaluate.at(0).getBodyPredicate().size() > 1) {
				if (rules_evaluate.at(0).getBodyPredicate().at(1).getName() != rules_evaluate.at(0).getHead().getName()) {
					ip.evaluateSpecificRules(rules_evaluate, db);
					counter = 1;
				}
				else {
					// run the rules atleast once then check to see if new tuples were added if so repeat
					do {
						oldCount = db.numTuples();
						ip.evaluateSpecificRules(rules_evaluate, db);
						newCount = db.numTuples();
						counter++;
					} while (oldCount != newCount);
				}
			}
			else {
				ip.evaluateSpecificRules(rules_evaluate, db);
				counter = 1;
			}
		}

		else {
			// run the rules atleast once then check to see if new tuples were added if so repeat
			do {
				oldCount = db.numTuples();
				ip.evaluateSpecificRules(rules_evaluate, db);
				newCount = db.numTuples();
				counter++;
			} while (oldCount != newCount);
		}


		// Output how many times the rules were processed and which rules were evaluated 
		cout << counter << " passes: ";
		for (long unsigned int j = 0; j < ruleOrder.at(i).size(); ++j) {
			cout << 'R' << ruleOrder.at(i).at(j);
			if (j < ruleOrder.at(i).size() - 1) {
				cout << ',';
			}
		}
		cout << endl; 

	}

	// Evaluate all the queries 
	cout << endl << "Query Evaluation" << endl;
	ip.evaluateAllQueries(db);

	// Print the results 
	cout << ip.toString();

}
