#pragma once
#include <iostream>
#include <set>
#include "Scheme.h"
#include "Tuple.h"
#include <algorithm> 
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

   bool addTuple(const Tuple& tuple)  {
       if (tuples.insert(tuple).second)
           return true;
       else
           return false; 
    }

   void getTuples(set<Tuple>& tups) {
       tups = tuples; 
   }
   

    string getName() {
        return name; 
    }

    Scheme getScheme() {
        return scheme; 
    }

    bool emptyTuples() {
        return tuples.empty(); 
    }

    int getTuplesSize() {
        return tuples.size(); 
    }

    static bool joinable(const Scheme& leftScheme, const Scheme& rightScheme, const Tuple& leftTuple, const Tuple& rightTuple) {

        for (unsigned leftIndex = 0; leftIndex < leftScheme.size(); leftIndex++) {
            const string& leftName = leftScheme.at(leftIndex);
            const string& leftValue = leftTuple.at(leftIndex);
            //cout << "left name: " << leftName << " value: " << leftValue << endl;

            for (unsigned rightIndex = 0; rightIndex < rightScheme.size(); rightIndex++) {
                const string& rightName = rightScheme.at(rightIndex);
                const string& rightValue = rightTuple.at(rightIndex);
                //cout << "right name: " << rightName << " value: " << rightValue << endl;
                
                if (leftName == rightName && leftValue != rightValue)
                    return false; 
            }
        }

        return true;
    }


    Relation join(const Relation& right) {
        const Relation& left = *this;
        Relation result;

        // add all of left schemes 
        for (long unsigned int i = 0; i < scheme.size(); ++i) {
            result.scheme.push_back(scheme.at(i)); 
 
        }

        // add all of right schemes that arn't already included from left scheme
        for (long unsigned int i = 0; i < right.scheme.size(); ++i) {
            if (find(result.scheme.begin(), result.scheme.end(), right.scheme.at(i)) == result.scheme.end()) {
                result.scheme.push_back(right.scheme.at(i)); 
            }

        }
       
        set<Tuple>::iterator itrl;
        for (itrl = tuples.begin(); itrl != tuples.end(); ++itrl) {
            Tuple leftTuple = *itrl; 
            //cout << "left tuple: " << leftTuple.toString(left.scheme) << endl;

            set<Tuple>::iterator itrr; 
            for (itrr = right.tuples.begin(); itrr != right.tuples.end(); ++itrr) {
                Tuple rightTuple = *itrr; 
                //cout << "right tuple: " << rightTuple.toString(right.scheme) << endl;
                //cout << joinable(scheme, right.scheme, *itr2, *itr1) << endl << endl;

                // if tuples are joinable then join them 
                if (joinable(left.scheme, right.scheme, leftTuple, rightTuple)) {
                    vector<string> preTuple; 

                    // include entire left tuple 
                    for (long unsigned int i = 0; i < leftTuple.size(); ++i) {
                        preTuple.push_back(leftTuple.at(i)); 
                    }

                    // only include input from right tuple if its corresponding scheme wasn't joined with the first 
                    for (long unsigned int i = 0; i < right.scheme.size(); ++i) {
                        if (find(scheme.begin(), scheme.end(),  right.scheme.at(i)) == scheme.end())
                            preTuple.push_back(rightTuple.at(i)); 
                        
                    }

                    result.tuples.insert(preTuple); 
                    
                }

            }
        }

        return result;
    }

    // Selects the tuples that have the given value at the given index 
    Relation select1(int index, const string& value) const {

        Relation result(name, scheme);
 
        set<Tuple>::iterator itr; 
        for (itr = tuples.begin(); itr != tuples.end(); ++itr) {
            Tuple tp = *itr; 
            if (tp.at(index) == value) {
                result.addTuple(tp); 
            }
        }
        return result;
    }

    // selects tuple with value of previos indexed value of that variable 
    Relation select2(int index, const string& value) const {

        Relation result(name, scheme);
        string verify;


        set<Tuple>::iterator itr;
        for (itr = tuples.begin(); itr != tuples.end(); ++itr) {
            Tuple tp = *itr;
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
        rel.name = name; 
        rel.scheme = order; 
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

    void unite(Relation rel) {
        set<Tuple>::iterator itr; 
        for (itr = rel.tuples.begin(); itr != rel.tuples.end(); ++itr) {
            if (tuples.insert(*itr).second) {
                tuples.insert(*itr);
                Tuple tup = *itr; 
                cout << "  " << tup.toString(rel.scheme) << endl;
            }
        }
    }



};
