#ifndef MINHEAP_H
#define MINHEAP_H

#include "../EngineCore/DryRunStep.h"
#include "../EngineCore/VisualizerEngine.h"
#include <vector>
#include <iostream>
using namespace std;

class MinHeap {
private:
    vector<int> heap;
    VisualizerEngine* engine;

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    MinHeap(VisualizerEngine* eng) : engine(eng) {}

    void insert(int value);
    void extractMin();
    void printHeap() const;
};

#endif
