#pragma once
#include <vector>
#include <string>
#include <optional>
#include <memory>
#include "../EngineCore/VisualizerEngine.h"

class BST {
private:
    struct Node {
        int value;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        Node(int v) : value(v), left(nullptr), right(nullptr) {}
    };

public:
    BST(VisualizerEngine* eng) : engine(eng) {}
    
    void insert(int value);
    void remove(int value);
    bool search(int value) const;
    
    // We will use a pre-order traversal for the DryRunStep report.
    // The visualizer will read the 'treeData' for the node positions.
    std::vector<int> data() const;
    
    // Structure to hold the necessary data for the visualizer to draw the tree.
    struct NodeData {
    int value;
    int depth;
    int index;
    int parentIndex; // <-- NEW: Horizontal index of the parent
};


    
    // This is the CRITICAL data structure the visualizer will use.
    std::vector<NodeData> getTreeData() const;

private:
    std::shared_ptr<Node> root = nullptr;
    VisualizerEngine* engine;

    // Helper functions
    std::shared_ptr<Node> insertRecursive(std::shared_ptr<Node> node, int value, std::vector<int>& traversal);
    std::shared_ptr<Node> removeRecursive(std::shared_ptr<Node> node, int value, std::vector<int>& traversal);
    std::shared_ptr<Node> findMin(std::shared_ptr<Node> node);

    // 1. The original public-facing function (wrapper) - NO parentValue
    void populateTreeData(std::shared_ptr<Node> node, int depth, int& index, std::vector<NodeData>& data) const;
    
    // 2. The new recursive helper (CRITICAL FIX) - WITH parentValue
    void populateTreeDataRecursive(std::shared_ptr<Node> node, int depth, int& index, std::vector<NodeData>& data, int parentValue) const;

    void preOrderTraversal(std::shared_ptr<Node> node, std::vector<int>& result) const;
};