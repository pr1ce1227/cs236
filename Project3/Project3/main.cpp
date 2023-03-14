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
	ip.UpdateDatabase(program.getDP()); 

	// creates a database with the updated data 
	Database db = ip.getDatabase(); 
	//cout << db.toString();  // can be used to debug and verify database 

	// Evaluate all the queries 
	ip.evaluateAllQueries(); 
	
	// Print the results 
	cout << ip.toString(); 
	
}