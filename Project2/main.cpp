#include <iostream>
#include <fstream>
#include <string>
#include "Parser.h"
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

	// Print tokens to the terminal 
	Parser tokens = file.scanToken();
	tokens.parse(); 

}


