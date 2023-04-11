#pragma once
#include <iostream>
#include <set>
#include <sstream>
#include <string>
using namespace std; 

class Node {

private:

    set<int> adjacentNodeIDs;
    bool visited = false; 

public:

    void addEdge(int adjacentNodeID) {
        adjacentNodeIDs.insert(adjacentNodeID);
    }

    set<int> getEdges() {
        return adjacentNodeIDs;
    }

    bool getVisited() {
        return visited; 
    }

    void setVisited(bool value) {
        visited = value; 
    }

    string toString() {
        stringstream out; 
        for (auto& nodeID : adjacentNodeIDs) 
            out << 'R' << nodeID << ',';
        return out.str().substr(0,out.str().length() - 1); 
    }

};
