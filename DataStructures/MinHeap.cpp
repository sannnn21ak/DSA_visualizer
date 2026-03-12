#include "MinHeap.h"

// Move the inserted element up to maintain heap property
void MinHeap::heapifyUp(int index) {
    if (index == 0) return;
    int parent = (index - 1) / 2;
    if (heap[index] < heap[parent]) {
        swap(heap[index], heap[parent]);
        heapifyUp(parent);
    }
}

// Move the root element down to maintain heap property
void MinHeap::heapifyDown(int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int smallest = index;

    if (left < heap.size() && heap[left] < heap[smallest])
        smallest = left;
    if (right < heap.size() && heap[right] < heap[smallest])
        smallest = right;

    if (smallest != index) {
        swap(heap[index], heap[smallest]);
        heapifyDown(smallest);
    }
}

// Insert a new value
void MinHeap::insert(int value) {
    engine->addStep(DryRunStep("insert", "MinHeap", heap.size(), value));
    heap.push_back(value);
    heapifyUp(heap.size() - 1);
}

// Remove the smallest element
void MinHeap::extractMin() {
    if (heap.empty()) return;

    engine->addStep(DryRunStep("extractMin", "MinHeap", 0, heap[0]));
    heap[0] = heap.back();
    heap.pop_back();
    heapifyDown(0);
}

// Print heap
void MinHeap::printHeap() const {
    cout << "Heap: ";
    for (int val : heap)
        cout << val << " ";
    cout << endl;
}
