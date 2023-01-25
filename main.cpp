#include <iostream>
#include <fstream>
#include <string>
#include "Token.h"
#include "Scanner.h"
#include <list>
using namespace std; 

int main(int argc, char* argv[]) {
	string fileName = argv[1]; 
	ifstream in; 
	in.open(fileName); 
	stringstream ss; 
	ss << in.rdbuf(); 
	string input = ss.str(); 
	in.close(); 

	Scanner file(input, 1); 
	static list<Token> tokens; 

	tokens = file.scanToken(); 
	for (list<Token>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
		string print = it->toString(); 
		cout << print << endl; 
	}

}


