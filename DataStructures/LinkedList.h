#pragma once
#include <vector>
#include <string>
#include <optional>
#include "../EngineCore/VisualizerEngine.h"

class LinkedList {
public:
    LinkedList(VisualizerEngine* eng) : engine(eng) {}
    
    void insertFront(int value);
    void insertBack(int value);
    void insertAt(int index, int value);
    
    std::optional<int> removeFront();
    std::optional<int> removeBack();
    std::optional<int> removeAt(int index);
    
    std::vector<int> data() const { return arr; }

private:
    // This vector holds the *values* in list order for reporting to the visualizer.
    std::vector<int> arr; 
    VisualizerEngine* engine;

    // NOTE: A real LinkedList would have a Node* head and the struct Node definition here.
};