#pragma once
#include <vector>
#include <string>
#include <optional>
#include "../EngineCore/VisualizerEngine.h"

class Stack {
public:
    Stack(VisualizerEngine* eng) : engine(eng) {}
    
    void push(int value);
    // Returns the popped value (if present)
    std::optional<int> pop(); 
    // Returns the top value (read-only)
    std::optional<int> top() const;
    
    std::vector<int> data() const { return arr; }

private:
    std::vector<int> arr; 
    VisualizerEngine* engine;
};