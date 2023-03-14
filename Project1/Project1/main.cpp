#include <iostream>
#include <fstream>
#include <string>
#include "Token.h"
#include "Scanner.h"
#include <vector>
using namespace std; 

int main(int argc, char* argv[]) {

	// Input file into a string 
	string fileName = argv[1]; 
	ifstream in; 
	in.open(fileName); 
	stringstream ss; 
	ss << in.rdbuf(); 
	string input = ss.str(); 
	in.close(); 

	// Scan file for tokens 
	int numTokens = 0; 
	Scanner file(input); 
	static vector<Token> tokens; 

	// Print tokens to the terminal 
	tokens = file.scanToken(); 
	for (vector<Token>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
		string print = it->toString(); 
		cout << print << endl; 
		numTokens++; 
	}

	cout << "Total Tokens = " << numTokens << endl; 

}


