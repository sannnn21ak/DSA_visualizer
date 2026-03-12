#include "Queue.h"
#include <algorithm>

void Queue::enqueue(int value) {
    arr.push_back(value); // New element added to the back (enqueue)
    
    if (engine) engine->addStep({ 
        "enqueue", 
        "Queue", 
        "value=" + std::to_string(value),
        arr,
        "" 
    });
}

std::optional<int> Queue::dequeue() {
    if (arr.empty()) {
        if (engine) engine->addStep({
            "dequeue",
            "Queue",
            "Queue is empty",
            arr,
            "Operation failed: Queue is empty."
        });
        return std::nullopt;
    }
    
    int v = arr.front();
    // Remove the element at the front (index 0) - This is the O(N) part of vector-as-queue
    arr.erase(arr.begin()); 
    
    if (engine) engine->addStep({ 
        "dequeue", 
        "Queue", 
        "value=" + std::to_string(v) + " removed",
        arr, // Pass the NEW state of the array after removal
        "" 
    });
    return v;
}

std::optional<int> Queue::front() const {
    if (arr.empty()) {
        // No step is added for 'front' as it's a read-only operation
        return std::nullopt;
    }
    return arr.front();
}