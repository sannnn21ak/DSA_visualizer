#include "BST.h"
#include <algorithm>
#include <sstream>

// --- Insertion Logic ---

std::shared_ptr<BST::Node> BST::insertRecursive(std::shared_ptr<Node> node, int value, std::vector<int>& traversal) {
    if (!node) {
        return std::make_shared<Node>(value);
    }
    
    traversal.push_back(node->value); // Record traversal path

    if (value < node->value) {
        node->left = insertRecursive(node->left, value, traversal);
    } else if (value > node->value) {
        node->right = insertRecursive(node->right, value, traversal);
    }
    // If equal, ignore (no duplicates)
    return node;
}

void BST::insert(int value) {
    std::vector<int> traversal;
    root = insertRecursive(root, value, traversal);
    
    // Report traversal path to the AI for explanation
    std::stringstream ss;
    ss << "Traversed: ";
    for (size_t i = 0; i < traversal.size(); ++i) {
        ss << traversal[i];
        if (i < traversal.size() - 1) ss << " -> ";
    }
    
    if (engine) engine->addStep({ 
        "insert", 
        "BST", 
        "value=" + std::to_string(value) + ", Path: " + ss.str(),
        data(), // Report linear traversal (for compatibility)
        "" 
    });
}

// --- Removal/Search (Placeholders) ---

void BST::remove(int value) {
    // For now, only report the operation. Full removal logic is complex.
    if (engine) engine->addStep({
        "remove", 
        "BST", 
        "Operation not yet implemented: " + std::to_string(value),
        data(),
        "Removal logic is complex. Feature coming soon."
    });
}

bool BST::search(int value) const {
    // Placeholder
    return false;
}

// --- Data Reporting for Visualizer (Hierarchical Position & Edge Tracking) ---

// **NEW:** Recursive helper to calculate position and pass the parent's value down.
// We store the parent's VALUE, which QtVisuals will use to find the parent's position (index).
void BST::populateTreeDataRecursive(std::shared_ptr<Node> node, int depth, int& index, std::vector<NodeData>& data, int parentValue) const {
    if (!node) return;
    
    // 1. Recurse Left (passing current node's value as the new parent value)
    populateTreeDataRecursive(node->left, depth + 1, index, data, node->value);
    
    // 2. Process Current Node (In-Order Traversal)
    data.push_back({
        node->value, 
        depth, 
        index, 
        parentValue // Store the parent's VALUE (not index!)
    });
    index++;
    
    // 3. Recurse Right
    populateTreeDataRecursive(node->right, depth + 1, index, data, node->value);
}

// Public wrapper function (matches the BST.h declaration)
void BST::populateTreeData(std::shared_ptr<Node> node, int depth, int& index, std::vector<NodeData>& data) const {
    // Start recursion with -1 as the parent value for the root.
    populateTreeDataRecursive(node, depth, index, data, -1); 
}

// Returns the full tree structure needed by the visualizer
std::vector<BST::NodeData> BST::getTreeData() const {
    std::vector<NodeData> data;
    int index = 0;
    // Calls the updated populateTreeData wrapper.
    populateTreeData(this->root, 0, index, data); 
    return data;
}

// Reports the 4-tuple: (Value, Depth, Index, ParentValue)


// Placeholder traversal is no longer needed but kept for completeness if other parts relied on it.
std::vector<int> BST::data() const {
    // This function must now return the 4-tuple data to fix the visualization
    std::vector<NodeData> treeData = getTreeData();
    std::vector<int> encodedData;
    
    for (const auto& node : treeData) {
        encodedData.push_back(node.value);
        encodedData.push_back(node.depth);
        encodedData.push_back(node.index);
        encodedData.push_back(node.parentIndex); 
    }
    return encodedData;
}