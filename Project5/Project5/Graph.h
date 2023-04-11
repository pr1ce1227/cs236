#pragma once
#include <iostream>
#include <map>
#include "Node.h"
#include <sstream>
#include <algorithm>
#include <vector>
using namespace std; 

class Graph {

private:

    map<int, Node> nodes;
    int size; 

public:

    Graph(int size1) {
        size = size1; 
        for (int nodeID = 0; nodeID < size1; nodeID++)
            nodes[nodeID] = Node();
    }

    void addEdge(int fromNodeID, int toNodeID) {
        nodes[fromNodeID].addEdge(toNodeID);
    }

    Graph reverseGraph() {
        Graph rev_graph(size);
        for (auto itr_from = nodes.begin(); itr_from != nodes.end(); ++itr_from) {
            set<int> edges = itr_from->second.getEdges(); 
            for (auto itr_to = edges.begin(); itr_to != edges.end(); ++itr_to) {
                rev_graph.addEdge(*itr_to, itr_from->first); 
            }
        }
        return rev_graph; 
    }

    vector<int> getPostOrder() {
        vector<int> postOrder; 
        vector<int> postOrder_add; 

        for (long unsigned int j = 0; j < nodes.size(); ++j) {
            if (!nodes[j].getVisited()) {
                postOrder_add = depthFirstSearch(j); 
                for (long unsigned int i = 0; i < postOrder_add.size(); ++i) {
                    postOrder.push_back(postOrder_add.at(i)); 
                }
            }
        }

        return postOrder; 
    }

    vector<vector<int>> dfsForward(vector<int> order) {
        vector<vector<int>> postOrder;
        vector<int> postOrder_add;

        for (int j = nodes.size() - 1; j >= 0; --j) {
            if (!nodes[order.at(j)].getVisited()) {
                postOrder_add = depthFirstSearch(order.at(j));
                postOrder.push_back(postOrder_add);
            }
        }

        return postOrder; 
    }

    vector<int> depthFirstSearch(int root) {
        vector<int> postOrder; 
        vector<int> postOrder_add; 
        nodes[root].setVisited(true); 
        set<int> edges = nodes[root].getEdges();

        for (auto itr = edges.begin(); itr != edges.end(); ++itr) {
            if (!nodes[*itr].getVisited()) {
                postOrder_add = depthFirstSearch(*itr);
                for (long unsigned int k = 0; k < postOrder_add.size(); ++k) {
                    postOrder.push_back(postOrder_add.at(k)); 
                }
            }
        }
        postOrder.push_back(root); 
        return postOrder; 
    }


    string toString() {
        stringstream out; 
        for (auto& pair : nodes) {
            int nodeID = pair.first;
            Node node = pair.second;
            out << 'R' << nodeID << ':' << node.toString() << endl; 
        }
        return out.str(); 
    }

};