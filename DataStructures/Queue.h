#pragma once
#include <vector>
#include <string>
#include <optional>
#include "../EngineCore/VisualizerEngine.h"

class Queue {
public:
    Queue(VisualizerEngine* eng) : engine(eng) {}
    
    void enqueue(int value);
    // Returns the dequeued value (if present)
    std::optional<int> dequeue(); 
    // Returns the front value (read-only)
    std::optional<int> front() const;
    
    std::vector<int> data() const { return arr; }

private:
    // Using a vector is simplest for this implementation
    std::vector<int> arr; 
    VisualizerEngine* engine;
};