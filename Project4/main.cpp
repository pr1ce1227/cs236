#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Parser.h"
#include "Parameter.h"
#include "Token.h"
#include "Scanner.h"
#include "Scheme.h"
#include "Tuple.h"
#include "Database.h"
#include "Relation.h"
#include "Interpreter.h"
using namespace std;


int main(int argc, char* argv[]) {


	// Input file into a string called input
	string fileName = argv[1];
	ifstream in;
	in.open(fileName);
	stringstream ss;
	ss << in.rdbuf();
	string input = ss.str();
	in.close();

	// Scan file for tokens 
	Scanner file(input);

	// Parse tokens into facts, schemes, rules, queries and domains
	// Throws an error if invalid input or in the wrong order
	Parser program = file.scanToken();
	program.parse();

	// input datalog program data into interpreter to be sorted 
	Interpreter ip;
	DatalogProgram dp = program.getDP(); 
	Database db; 
	ip.UpdateDatabase(dp, db);
	//cout << db.toString();  // can be used to debug and verify database 

	cout << "Rule Evaluation" << endl; 
	int oldCount = 0; 
	int newCount = 0; 
	int counter = 0; 

	do{
		oldCount = db.numTuples(); 
		ip.evaluateRules(db);
		newCount = db.numTuples(); 
		counter++; 
	} while (oldCount != newCount);

	cout << endl << "Schemes populated after " << counter << " passes through the Rules." << endl << endl;

	// Evaluate all the queries 
	cout << "Query Evaluation" << endl; 
	ip.evaluateAllQueries(db);

	// Print the results 
	cout << ip.toString();

}