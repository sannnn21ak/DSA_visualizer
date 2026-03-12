#include "Stack.h"
#include <algorithm>

void Stack::push(int value) {
    arr.push_back(value); // New element added to the top/back
    
    if (engine) engine->addStep({ 
        "push", 
        "Stack", 
        "value=" + std::to_string(value),
        arr,
        "" 
    });
}

std::optional<int> Stack::pop() {
    if (arr.empty()) {
        if (engine) engine->addStep({
            "pop",
            "Stack",
            "Stack is empty",
            arr,
            "Operation failed: Stack is empty."
        });
        return std::nullopt;
    }
    
    int v = arr.back();
    arr.pop_back(); // Remove the element from the back (LIFO)
    
    if (engine) engine->addStep({ 
        "pop", 
        "Stack", 
        "value=" + std::to_string(v) + " removed",
        arr, 
        "" 
    });
    return v;
}

std::optional<int> Stack::top() const {
    if (arr.empty()) {
        return std::nullopt;
    }
    return arr.back();
}