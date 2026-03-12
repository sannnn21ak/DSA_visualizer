#include "HashMap.h"

HashMap::HashMap(int cap, VisualizerEngine* eng) : capacity(cap), engine(eng) {
    table.resize(capacity);
}

void HashMap::insert(int key, int value) {
    int idx = hashFunction(key);
    for (auto& kv : table[idx]) {
        if (kv.first == key) {
            kv.second = value;
            engine->addStep(DryRunStep("update", "HashMap", idx, value));
            return;
        }
    }
    table[idx].push_back({key, value});
    engine->addStep(DryRunStep("insert", "HashMap", idx, value));
}

void HashMap::remove(int key) {
    int idx = hashFunction(key);
    for (auto it = table[idx].begin(); it != table[idx].end(); ++it) {
        if (it->first == key) {
            int val = it->second;
            table[idx].erase(it);
            engine->addStep(DryRunStep("delete", "HashMap", idx, val));
            return;
        }
    }
}

int HashMap::get(int key) {
    int idx = hashFunction(key);
    for (auto& kv : table[idx]) {
        if (kv.first == key) return kv.second;
    }
    return -1; // not found
}

void HashMap::print() const {
    cout << "HashMap:" << endl;
    for (int i = 0; i < capacity; ++i) {
        cout << i << ": ";
        for (auto& kv : table[i]) {
            cout << "(" << kv.first << "," << kv.second << ") ";
        }
        cout << endl;
    }
}
