#include "Graph.h"
#include <algorithm>
#include <sstream>

void Graph::addNode(int value) {
    if (adjList.find(value) == adjList.end()) {
        adjList[value] = {}; // Add node with no neighbors
    }
    
    if (engine) engine->addStep({ 
        "addNode", 
        "Graph", 
        "value=" + std::to_string(value),
        data(), 
        "" 
    });
}

void Graph::addEdge(int node1, int node2, bool directed) {
    // Ensure both nodes exist
    if (adjList.find(node1) == adjList.end() || adjList.find(node2) == adjList.end()) {
        if (engine) engine->addStep({
            "addEdge",
            "Graph",
            "Missing Node",
            data(),
            "Operation failed: One or both nodes do not exist."
        });
        return;
    }
    
    // Add edge (node1 -> node2)
    adjList[node1].push_back(node2);
    
    if (!directed) {
        // Add reverse edge (node2 -> node1) for undirected graph
        adjList[node2].push_back(node1);
    }
    
    if (engine) engine->addStep({ 
        "addEdge", 
        "Graph", 
        "edge=" + std::to_string(node1) + "->" + std::to_string(node2) + (directed ? "(Directed)" : "(Undirected)"),
        data(), 
        "" 
    });
}

void Graph::removeNode(int value) {
    // 1. Remove the node from the main list
    adjList.erase(value);
    
    // 2. Remove all edges pointing TO this node
    for (auto& pair : adjList) {
        auto& neighbors = pair.second;
        neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), value), neighbors.end());
    }

    if (engine) engine->addStep({ 
        "removeNode", 
        "Graph", 
        "value=" + std::to_string(value),
        data(), 
        "" 
    });
}

// Reports a list of all nodes for the DryRunStep 'values' vector
std::vector<int> Graph::data() const {
    std::vector<int> nodes;
    for (const auto& pair : adjList) {
        nodes.push_back(pair.first);
    }
    return nodes;
}