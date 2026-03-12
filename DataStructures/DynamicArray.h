#pragma once
#include <vector>
#include <string>
#include "../EngineCore/DryRunStep.h"
#include "../EngineCore/VisualizerEngine.h"

class DynamicArray {
public:
    DynamicArray(VisualizerEngine* eng) : engine(eng) {}
    void insertAt(int index, int value);
    void pushBack(int value);
    void removeAt(int index);
    std::vector<int> data() const { return arr; }

private:
    std::vector<int> arr;
    VisualizerEngine* engine;
};
