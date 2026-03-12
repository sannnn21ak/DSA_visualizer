#include "DynamicArray.h"
#include <algorithm>

void DynamicArray::insertAt(int index, int value) {
    if (index < 0) index = 0;
    if (index > (int)arr.size()) index = arr.size();
    arr.insert(arr.begin() + index, value);
    if (engine) engine->addStep({ 
        "insert", 
        "DynamicArray", 
        "value=" + std::to_string(value) + " idx=" + std::to_string(index),
        arr,
        "" // ADDED: Initializing DryRunStep::message
    });
}

void DynamicArray::pushBack(int value) {
    arr.push_back(value);
    if (engine) engine->addStep({ 
        "push_back", 
        "DynamicArray", 
        "value=" + std::to_string(value),
        arr,
        "" // ADDED: Initializing DryRunStep::message
    });
}

void DynamicArray::removeAt(int index) {
    if (index >= 0 && index < (int)arr.size()) {
        int v = arr[index];
        arr.erase(arr.begin() + index);
        if (engine) engine->addStep({ 
            "remove", 
            "DynamicArray", 
            "value=" + std::to_string(v) + " idx=" + std::to_string(index),
            arr,
            "" // ADDED: Initializing DryRunStep::message
        });
    }
}