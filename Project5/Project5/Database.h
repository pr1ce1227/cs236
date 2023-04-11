#pragma once
#include "DatalogProgram.h"
#include "Relation.h"
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
using namespace std; 

class Database {
	friend class Interpreter; 
private: 
	map<string, Relation> relations; 

public: 
	Database(){}


	void addRelation(string name, Relation relation) {
		relations.insert({ name,relation }); 
	}

	int numTuples() {
		map<string, Relation>::iterator itr; 
		int count = 0; 
		for (itr = relations.begin(); itr != relations.end(); ++itr) {
			Relation rel = itr->second; 
			count += rel.getTuplesSize();
		}
		return count; 
	}


	string toString() {
		stringstream out; 
		map<string, Relation>::iterator itr;
		for (itr = relations.begin(); itr != relations.end(); ++itr) {
			out << itr->second.toString(); 
		}
		out << endl << endl; 

		return out.str(); 
	}

	

};
