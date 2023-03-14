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

	// Function runs through each data type and stores them in its private members 
	// If out of order or invalid input throws an error, Start symbol for grammer 
	void dataProgram() {

		// Add all the schemes
		if (tokenType() == SCHEMES) {
			match(SCHEMES);
			match(COLON);
			scheme();
			schemeList();
		}
		else {
			throw tokens.at(0);
		}

		// Add al the facts 
		if (tokenType() == FACTS) {
			match(FACTS);
			match(COLON);
			factList();
		}
		else {
			throw tokens.at(0);
		}

		// Add all the rules 
		if (tokenType() == RULES) {
			match(RULES);
			match(COLON);
			ruleList();
		}
		else {
			throw tokens.at(0);
		}

		// Add all the queries 
		if (tokenType() == QUERIES) {
			match(QUERIES);
			match(COLON);
			query();
			queryList();
		}
		else {
			throw tokens.at(0);
		}

		// Check for end of file 
		if (tokenType() == eof) {
			// lambda
		}
		else {
			throw tokens.at(0);
		}
	}

	// Checks each token to make sure its in the right syntax finished by added fact to DatalogProgram 
	void fact() {
		if (tokenType() == ID) {

			// Recreated predicate value for each recursive call 
			Predicate Pre(tokens.at(0).getValue());
			match(ID); 
			match(LEFT_PAREN);

			// Recursivly call param until finished, each fact param is also a domain 
			Parameter Par(tokens.at(0).getValue());
			Dp.addDomain(Par); 
			Pre.addParam(Par);
			match(STRING); 
			stringList(Pre); 

			// Check closing tokens of the fact 
			match(RIGHT_PAREN); 
			match(PERIOD); 
			Dp.addFact(Pre);
		}
		else {
			throw tokens.at(0);
		}
	}

	// recursivly calls itself until there are no more facts 
	void factList() {
		if (tokenType() == ID) {
			fact(); 
			factList(); 
		}
		else {
			// lambda
		}
	}

	// Creates a head predicate specific to the Rule predicate type
	void headPredicit(Rule& rul) {
		if (tokenType() == ID) {

			// Create new predicate for each recursive call, check front syntax of predicate
			Predicate Pre(tokens.at(0).getValue());
			match(ID); 
			match(LEFT_PAREN);

			// Recursivly add parameters until reaches the end 
			Parameter Par(tokens.at(0).getValue());
			Pre.addParam(Par);
			parameter(); 
			parameterList(Pre); 

			// Check closing syntax of parameter 
			match(RIGHT_PAREN);
			rul.setHead(Pre); 
		}
		else {
			throw tokens.at(0);
		}
	}

	// Continues to add Id's until there isn't one 
	void idList(Predicate& pre) {
		
		// Recursivly checks for id's until there isn't one 
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

	// Matches the current token to the input token 
	void match(TokenType t) {
		if (tokenType() == t)
			advanceToken();
		else
			throw tokens.at(0);
	}

	// calls the dataProgram function and runs print when finished,
	// placed here so only prints if the program was successful 
	void parse() {
		try {
			dataProgram();
			print(); 
		}
		catch (Token errorToken) {
			cout << "Failure!" << endl; 
			cout << "  " << errorToken.toString() << endl;
		}
	}

	// Checks for valid parameters 
	void parameter() {
		if (tokenType() == STRING) {
			match(STRING); 
		}
		else {
			match(ID); 
		}
	}

	// recursivly call parameter until they are all done 
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

	// Validate that sequence of token is a predicate then add to DatalogProgram 
	void predicate() {
		if (tokenType() == ID) {
			// Check for name(
			Predicate Pre(tokens.at(0).getValue());
			match(ID); 
			match(LEFT_PAREN);

			// Continue to add parameter until ) 
			Parameter Par(tokens.at(0).getValue());
			Pre.addParam(Par);
			parameter(); 
			parameterList(Pre); 

			// match ) then add to Dp 
			match(RIGHT_PAREN); 
			Dp.addQuerie(Pre);
		}
		else {
			throw tokens.at(0);
		}
	}

	// These predicate are specifics to the bodyPredicate vector inside of rules 
	void predicitBody(Rule& rul) {
		if (tokenType() == ID) {
			// Check for intial name( 
			Predicate Pre(tokens.at(0).getValue());
			match(ID);
			match(LEFT_PAREN);

			// Continue to add parameters 
			Parameter Par(tokens.at(0).getValue());
			Pre.addParam(Par);
			parameter();
			parameterList(Pre);

			// check for ) then add to rule which will then be added to dp 
			match(RIGHT_PAREN);
			rul.addBodyPredicate(Pre); 
		}
		else {
			throw tokens.at(0);
		}
	}

	// Cycle through all the predicates 
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

	// Prints out all of the schemes, facts rules, quereies and domains 
	void print() {
		cout << "Success!" << endl;
		cout << Dp.toStringSchemes();
		cout << Dp.toStringFacts(); 
		cout << Dp.toStringRules(); 
		cout << Dp.toStringQueries();
		cout << Dp.toStringDomains(); 
	 
	}

	// Is just a predicated that ends with ? 
	void query() {
		predicate(); 
		match(Q_MARK); 
	}

	// continue to add queries until empty 
	void queryList() {
		if (tokenType() == ID) {
			query();
			queryList();
		}
		else {
			// lambda
		}

	}

	// Rules need to build a head predicate followed by :- and all of predicate bodies
	void rule() {
		// Head predicate 
		Rule rul; 
		headPredicit(rul); 
		match(COLON_DASH);

		// Body predicates 
		Predicate Pre; 
		predicitBody(rul); 
		predicitList(rul); 

		// end with period and add to Dp 
		match(PERIOD); 
		Dp.addRule(rul); 
	}

	// Contiue to add rules until empty 
	void ruleList() {
		if (tokenType() == ID) {
			rule(); 
			ruleList(); 
		}
		else {
			// lambda
		}
	}

	// are formed with predicates 
	void scheme() {
		if (tokenType() == ID) {
			// check for name and openeing ( 
			Predicate Pre(tokens.at(0).getValue());
			match(ID);
			match(LEFT_PAREN);

			// continue to add parameters until empty 
			Parameter Par(tokens.at(0).getValue());
			Pre.addParam(Par); 
			match(ID);
			idList(Pre);

			// Close with ) and add to DP 
			match(RIGHT_PAREN);
			Dp.addScheme(Pre); 
		}
		else
			throw tokens.at(0);
	}

	// Continue to add schemes until empty 
	void schemeList() {
		if (tokenType() == ID) {
			scheme(); 
			schemeList(); 
		}
		else {
			// lambda
		}
	}

	// contiue to add strings until empty 
	void stringList(Predicate& pre) {
		if ((tokenType() == COMMA)) {
			match(COMMA);
			Parameter Par(tokens.at(0).getValue());
			Dp.addDomain(Par); 
			pre.addParam(Par);
			match(STRING); 
			stringList(pre); 
		}
		else {
			// lambda
		}

	}

	// Returns the type of the current token 
	TokenType tokenType() const {
		return tokens.at(0).getType();
	}
};
