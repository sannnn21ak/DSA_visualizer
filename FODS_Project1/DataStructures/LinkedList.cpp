#include "LinkedList.h"
#include <algorithm>
#include <stdexcept>

// --- Helper function for reporting ---
void LinkedList::insertAt(int index, int value) {
    if (index < 0) index = 0;
    if (index > (int)arr.size()) index = arr.size();
    
    // Simulating the insertion:
    arr.insert(arr.begin() + index, value);
    
    if (engine) engine->addStep({ 
        "insertAt", 
        "LinkedList", 
        "value=" + std::to_string(value) + " index=" + std::to_string(index),
        arr,
        "" 
    });
}

void LinkedList::insertFront(int value) {
    insertAt(0, value);
    // Overwrite the operation name to be more descriptive for the AI
    if (!engine->getSteps().empty()) {
        const_cast<DryRunStep&>(engine->getSteps().back()).operation = "insertFront";
    }
}

void LinkedList::insertBack(int value) {
    insertAt(arr.size(), value);
    // Overwrite the operation name to be more descriptive for the AI
    if (!engine->getSteps().empty()) {
        const_cast<DryRunStep&>(engine->getSteps().back()).operation = "insertBack";
    }
}

// --- Removal operations ---

std::optional<int> LinkedList::removeAt(int index) {
    if (index < 0 || index >= (int)arr.size()) {
        if (engine) engine->addStep({
            "removeAt",
            "LinkedList",
            "Index out of bounds",
            arr,
            "Operation failed: Index out of bounds."
        });
        return std::nullopt;
    }
    
    int v = arr[index];
    arr.erase(arr.begin() + index);
    
    if (engine) engine->addStep({ 
        "removeAt", 
        "LinkedList", 
        "value=" + std::to_string(v) + " index=" + std::to_string(index),
        arr,
        "" 
    });
    return v;
}

std::optional<int> LinkedList::removeFront() {
    auto result = removeAt(0);
    // Overwrite the operation name to be more descriptive for the AI
    if (result.has_value() && !engine->getSteps().empty()) {
        const_cast<DryRunStep&>(engine->getSteps().back()).operation = "removeFront";
    }
    return result;
}

std::optional<int> LinkedList::removeBack() {
    if (arr.empty()) return std::nullopt;
    auto result = removeAt(arr.size() - 1);
    // Overwrite the operation name to be more descriptive for the AI
    if (result.has_value() && !engine->getSteps().empty()) {
        const_cast<DryRunStep&>(engine->getSteps().back()).operation = "removeBack";
    }
    return result;
}