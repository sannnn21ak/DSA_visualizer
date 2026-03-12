#pragma once
#include <string>
#include <vector> // ✅ ADDED: Necessary for std::vector<int>

struct DryRunStep {
    std::string operation;      // e.g. "insert", "remove", "push_back"
    std::string dataStructure;  // e.g. "DynamicArray", "LinkedList"
    std::string details;        // Technical details used by the AI/engine (e.g., "value=5 index=2")
    std::vector<int> values;    // The actual state of the data structure (e.g., the array contents)
    std::string message;        // Human-readable message or notes (e.g., "Array resized")
};