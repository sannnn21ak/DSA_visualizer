#pragma once

#include <vector>
#include <list>
#include <string>
#include "../EngineCore/VisualizerEngine.h"

class HashTable {
private:
    std::vector<std::list<int>> table;
    int num_buckets;

    int hash(int value) {
        return value % num_buckets;
    }

    VisualizerEngine* engine;

    // --- ADD THIS FUNCTION ---
    /**
     * @brief Encodes the hash table into a single vector for the DryRunStep.
     * Format: [num_buckets, b0_val1, b0_val2, ..., -1, b1_val1, ..., -1, ...]
     * A -1 separator is added after each bucket's list.
     */
    std::vector<int> data() const {
        std::vector<int> encodedData;
        encodedData.push_back(num_buckets);
        
        for (const auto& bucket : table) {
            for (int value : bucket) {
                encodedData.push_back(value);
            }
            encodedData.push_back(-1); // Add -1 as a separator
        }
        return encodedData;
    }
    // --- END ADD ---

public:
    HashTable(int buckets, VisualizerEngine* eng) : engine(eng) {
        num_buckets = (buckets > 0) ? buckets : 5;
        table.resize(num_buckets);
    }

    void insert(int value) {
        int bucketIndex = hash(value);
        table[bucketIndex].push_back(value);
        
        if (engine) engine->addStep({ 
            "insert", 
            "HashTable", 
            "value=" + std::to_string(value) + ", hash=" + std::to_string(bucketIndex),
            data() // <-- PASS THE ENCODED DATA
        });
    }

    void remove(int value) {
        int bucketIndex = hash(value);
        table[bucketIndex].remove(value);
        
        if (engine) engine->addStep({ 
            "remove", 
            "HashTable", 
            "value=" + std::to_string(value) + ", hash=" + std::to_string(bucketIndex),
            data() // <-- PASS THE ENCODED DATA
        });
    }

    // This function is no longer needed by QtVisuals, but is fine to keep
    const std::vector<std::list<int>>& getTable() const {
        return table;
    }
    
    int getBucketCount() const {
        return num_buckets;
    }
};