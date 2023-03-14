#pragma once
#include <iostream>
#include <set>
#include "Scheme.h"
#include "Tuple.h"
#include <map>
using namespace std; 

class Relation {
  
private:
    string name;
    Scheme scheme;
    set<Tuple> tuples;

public:
    Relation() {}

    Relation(const string& name, const Scheme& scheme)
        : name(name), scheme(scheme) {
        tuples = {};
    }

    void addTuple(const Tuple& tuple)  {
        tuples.insert(tuple);
    }


    string getName() {
        return name; 
    }

    bool emptyTuples() {
        return tuples.empty(); 
    }

    int getTuplesSize() {
        return tuples.size(); 
    }

    // Selects the tuples that have the given value at the given index 
    Relation select(int index, const string& value) const {

        Relation result(name, scheme);
        string verify; 
 
 
        set<Tuple>::iterator itr; 
        for (itr = tuples.begin(); itr != tuples.end(); ++itr) {
            Tuple tp = *itr; 
            if (value.at(0) == '\'') {
                verify = value;
            }
            else
                verify = tp.at(stoi(value)); 
            if (tp.at(index) == verify) {
                result.addTuple(tp); 
            }
        }
        return result;
    }

    // Project to only grab input map in the inputed order 
    Relation project(map<string,int> vars, vector<string> order) {
        Relation rel;  
        set<Tuple>::iterator itr;
        for (itr = tuples.begin(); itr != tuples.end(); ++itr) {
            vector<string> tuple_values;
            vector<string> preTuple;

            for (long unsigned int i = 0; i < order.size(); ++i) {
                int ind = vars.at(order.at(i)); 
                tuple_values = *itr; 
                preTuple.push_back(tuple_values.at(ind));
            }
            rel.addTuple(preTuple); 
        }
        return rel; 
    }

    // Renames the schemes to match the query 
    Relation rename(Relation relation, vector<string> order) {
        Relation rel = relation; 
        rel.scheme = order; 
        return rel; 
    }

    // Used for debugging only 
    string toString() {
        stringstream out;
        out << name << ':' << endl; 
        for (long unsigned int i = 0; i < scheme.size(); ++i) {
            out << scheme.at(i) << ' ';
        }
        out << endl; 

        set<Tuple>::iterator itr;
        for (itr = tuples.begin(); itr != tuples.end(); ++itr) {
            vector<string> str = *itr; 
            for (long unsigned int i = 0; i < str.size(); ++i) {
                    out << str.at(i) << ' ';
                
            }
            out << endl;
        }
        out << endl; 
        return out.str();
    }

    string toStringTuples() {
        set<Tuple>::iterator itr; 
        stringstream out; 
        for (itr = tuples.begin(); itr != tuples.end(); ++itr) {
            vector<string> vars = *itr;
            out << "  "; 
            for (long unsigned int i = 0; i < vars.size(); ++i) {
                out << scheme.at(i) << '=' << vars.at(i);
                if (i < vars.size() - 1)
                    out << ", "; 
            }
            out << endl; 
        }
        return out.str(); 
    }



};
