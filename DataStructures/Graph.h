#pragma once
#include <vector>
#include <string>
#include <optional>
#include <map>
#include "../EngineCore/VisualizerEngine.h"

class Graph {
public:
    Graph(VisualizerEngine* eng) : engine(eng) {}
    
    void addNode(int value);
    void addEdge(int node1, int node2, bool directed = false);
    void removeNode(int value);
    
    // Structure to hold the graph data (adjacency list) for reporting
    using AdjacencyList = std::map<int, std::vector<int>>;
    
    // We report the node values for the DryRunStep's 'values' vector
    std::vector<int> data() const;
    
    // CRITICAL: The visualizer needs the full graph structure
    AdjacencyList getAdjacencyList() const { return adjList; }

private:
    AdjacencyList adjList; 
    VisualizerEngine* engine;
};