#pragma once
#include "Token.h"
#include "Predicate.h"
#include "Rule.h"
#include "Parameter.h"
#include "DatalogProgram.h"
#include <vector>

class Parser {

private:
	DatalogProgram Dp; 
	vector<Token> tokens;

public:
	Parser(const vector<Token>& tokens) : tokens(tokens) { }

	DatalogProgram getDP() {
		return Dp; 
	}

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

		if (tokenType() == FACTS) {
			match(FACTS); 
			match(COLON); 
			factList(); 
		}

		if (tokenType() == RULES) {
			match(RULES); 
			match(COLON); 
			ruleList();
		}

		if (tokenType() == QUERIES) {
			match(QUERIES); 
			match(COLON); 
			query(); 
			queryList(); 
		}

		if (tokenType() == eof) {
			// lambda
		}

		else {
			throw tokens.at(0);
		}
	}

	void fact() {
		if (tokenType() == ID) {
			Predicate Pre(tokens.at(0).getValue());
			match(ID); 
			match(LEFT_PAREN);
			Parameter Par(tokens.at(0).getValue());
			Pre.addParam(Par);
			match(STRING); 
			stringList(Pre); 
			match(RIGHT_PAREN); 
			match(PERIOD); 
			Dp.addFact(Pre);
		}
		else {
			throw tokens.at(0);
		}
	}

	void factList() {
		if (tokenType() == ID) {
			fact(); 
			factList(); 
		}
		else {
			// lambda
		}
	}

	void headPredicit(Rule& rul) {
		if (tokenType() == ID) {
			Predicate Pre(tokens.at(0).getValue());
			match(ID); 
			match(LEFT_PAREN); 
			Parameter Par(tokens.at(0).getValue());
			Pre.addParam(Par);
			parameter(); 
			parameterList(Pre); 
			match(RIGHT_PAREN);
			rul.setHead(Pre); 
		}
		else {
			throw tokens.at(0);
		}
	}

	void idList(Predicate& pre) {
		if (tokenType() == COMMA) {
			match(COMMA);
			Parameter Par(tokens.at(0).getValue());
			pre.addParam(Par); 
			match(ID);
			idList(pre);
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

	void parameterList(Predicate& pre) {
		if (tokenType() == COMMA) {
			match(COMMA);
			Parameter Par(tokens.at(0).getValue());
			pre.addParam(Par);
			parameter(); 
			parameterList(pre); 
		}
		else {
			//lambda
		}
	}

	void predicit() {
		if (tokenType() == ID) {
			Predicate Pre(tokens.at(0).getValue());
			match(ID); 
			match(LEFT_PAREN);
			Parameter Par(tokens.at(0).getValue());
			Pre.addParam(Par);
			parameter(); 
			parameterList(Pre); 
			match(RIGHT_PAREN); 
			Dp.addQuerie(Pre);
		}
		else {
			throw tokens.at(0);
		}
	}

	void predicitBody(Rule& rul) {
		if (tokenType() == ID) {
			Predicate Pre(tokens.at(0).getValue());
			match(ID);
			match(LEFT_PAREN);
			Parameter Par(tokens.at(0).getValue());
			Pre.addParam(Par);
			parameter();
			parameterList(Pre);
			match(RIGHT_PAREN);
			rul.addBodyPredicate(Pre); 
		}
		else {
			throw tokens.at(0);
		}
	}

	void predicitList(Rule& rul) {
		if (tokenType() == COMMA) {
			match(COMMA); 
			predicitBody(rul); 
			predicitList(rul); 
		}
		else {
			// lambda
		}
	}

	void print() {
		cout << "Success!" << endl;
		cout << Dp.toStringSchemes();
		cout << endl; 
		cout << Dp.toStringFacts(); 
		cout << endl; 
		cout << Dp.toStringRules(); 
		cout << endl; 
		cout << Dp.toStringQueries(); 
	 
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
		Rule rul; 
		headPredicit(rul); 
		match(COLON_DASH); 
		Predicate Pre; 
		predicitBody(rul); 
		predicitList(rul); 
		match(PERIOD); 
		Dp.addRule(rul); 
	}

	void ruleList() {
		if (tokenType() == ID) {
			rule(); 
			ruleList(); 
		}
		else {
			// lambda
		}
	}

	void scheme() {
		if (tokenType() == ID) {
			Predicate Pre(tokens.at(0).getValue());
			match(ID);
			match(LEFT_PAREN);
			Parameter Par(tokens.at(0).getValue());
			Pre.addParam(Par); 
			match(ID);
			idList(Pre);
			match(RIGHT_PAREN);
			Dp.addScheme(Pre); 
		}
		else
			throw tokens.at(0);
	}

	void schemeList() {
		if (tokenType() == ID) {
			scheme(); 
			schemeList(); 
		}
		else {
			// lambda
		}
	}

	void stringList(Predicate& pre) {
		if ((tokenType() == COMMA)) {
			match(COMMA);
			Parameter Par(tokens.at(0).getValue());
			pre.addParam(Par);
			match(STRING); 
			stringList(pre); 
		}
		else {
			// lambda
		}

	}

	TokenType tokenType() const {
		return tokens.at(0).getType();
	}
};
