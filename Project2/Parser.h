#pragma once
#include "Token.h"
#include <vector>

class Parser {

private:
	vector<Token> tokens;

public:
	Parser(const vector<Token>& tokens) : tokens(tokens) { }


	void advanceToken() {
		tokens.erase(tokens.begin());
	}

	void idList() {
		if (tokenType() == COMMA) {
			match(COMMA);
			match(ID);
			idList();
		}
		else {
			// lambda
		}
	}

	void match(TokenType t) {
		cout << "match: " << t << endl;
		if (tokenType() == t)
			advanceToken();
		else
			cout << "Syntax error" << endl;
	}

	void scheme() {
		if (tokenType() == ID) {
			match(ID); 
			match(LEFT_PAREN); 
			match(ID); 
			idList(); 
			match(RIGHT_PAREN); 
		}
		else
			throwError();
	}

	void throwError() {
		cout << "error" << endl;
	}


	TokenType tokenType() const {
		return tokens.at(0).getType();
	}
};
