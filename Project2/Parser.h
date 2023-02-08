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

	void dataProgram() {
		if (tokenType() == SCHEMES) {
			match(SCHEMES); 
			match(COLON); 
			scheme(); 
			schemeList(); 
		}
		else if (tokenType() == FACTS) {
			match(FACTS); 
			match(COLON); 
			factList(); 
		}
		else if (tokenType() == RULES) {
			match(RULES); 
			match(COLON); 
			ruleList();
		}
		else if (tokenType() == QUERIES) {
			match(QUERIES); 
			match(COLON); 
			query(); 
			queryList(); 
		}
		else if (tokenType() == eof) {
			// lambda
		}
		else {
			throw tokens.at(0);
		}
	}

	void fact() {
		if (tokenType() == ID) {
			match(ID); 
			match(LEFT_PAREN); 
			match(STRING); 
			stringList(); 
			match(RIGHT_PAREN); 
			match(PERIOD); 
		}
		else {
			throw tokens.at(0);
		}
	}

	void factList() {
		if (tokenType() == FACTS) {
			fact(); 
			factList(); 
		}
		else {
			// lambda
		}
	}

	void headPredicit() {
		if (tokenType() == ID) {
			match(ID); 
			match(LEFT_PAREN); 
			parameter(); 
			parameterList(); 
			match(RIGHT_PAREN); 
		}
		else {
			throw tokens.at(0);
		}
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
		if (tokenType() == t)
			advanceToken();
		else
			throw tokens.at(0);
	}

	void parse() {
		try {
			dataProgram();
		}
		catch (Token errorToken) {
			cout << "Failure!" << endl; 
			cout << "  " << errorToken.toString();
		}
	}

	void parameter() {
		if (tokenType() == STRING) {
			match(STRING); 
		}
		else {
			match(ID); 
		}
	}

	void parameterList() {
		if (tokenType() == COMMA) {
			match(COMMA); 
			parameter(); 
			parameterList(); 
		}
		else {
			//lambda
		}
	}

	void predicit() {
		if (tokenType() == ID) {
			match(ID); 
			match(LEFT_PAREN);
			parameter(); 
			parameterList(); 
			match(RIGHT_PAREN); 
		}
		else {
			throw tokens.at(0);
		}
	}

	void predicitList() {
		if (tokenType() == COMMA) {
			match(COMMA); 
			predicit(); 
			predicitList(); 
		}
		else {
			// lambda
		}
	}

	void query() {
		predicit(); 
		match(Q_MARK); 
	}

	void queryList() {
		if (tokenType() == QUERIES) {
			query();
			queryList();
		}
		else {
			// lambda
		}

	}

	void rule() {
		headPredicit(); 
		match(COLON_DASH); 
		predicit(); 
		predicitList(); 
		match(PERIOD); 
	}

	void ruleList() {
		if (tokenType() == RULES) {
			rule(); 
			ruleList(); 
		}
		else {
			// lambda
		}
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
			throw tokens.at(0);
	}

	void schemeList() {
		if (tokenType() == SCHEMES) {
			scheme(); 
			schemeList(); 
		}
		else {
			// lambda
		}
	}

	void stringList() {
		if ((tokenType() == COMMA)) {
			match(COMMA);
			match(STRING); 
			stringList(); 
		}
		else {
			// lambda
		}

	}

	TokenType tokenType() const {
		return tokens.at(0).getType();
	}
};
