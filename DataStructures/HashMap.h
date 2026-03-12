#ifndef HASHMAP_H
#define HASHMAP_H

#include "../EngineCore/DryRunStep.h"
#include "../EngineCore/VisualizerEngine.h"
#include <vector>
#include <list>
#include <utility>
#include <iostream>

using namespace std;

class HashMap {
private:
    vector<list<pair<int, int>>> table;
    int capacity;
    VisualizerEngine* engine;

    int hashFunction(int key) {
        return key % capacity;
    }

public:
    HashMap(int cap, VisualizerEngine* eng);
    void insert(int key, int value);
    void remove(int key);
    int get(int key);
    void print() const;
};

#endif
