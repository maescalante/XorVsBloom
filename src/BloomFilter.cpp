#include <iostream>
#include "BloomFilter.h"
#include "hashUtils.cpp"
#include "helpers.cpp"
#include <vector>



using namespace std;

template<typename T>
BloomFilter<T>::BloomFilter(size_t bitsPerEntry): bitsPerEntry(bitsPerEntry) {
}

template<typename T>
void BloomFilter<T>::build(vector<uint64_t>& keys) {
    size_t numKeys = keys.size();
    int numHashFunctions = static_cast<int>(ceil(log(2) * this->bitsPerEntry));
    int64_t filterSize = this->bitsPerEntry * numKeys;
    
    this->filter.resize(filterSize);
    this->hashSeeds.resize(numHashFunctions);

    for (int i = 0; i < numHashFunctions; ++i) {
        this->hashSeeds[i] = generateRandomUInt64();
    }

    for (size_t i = 0; i < numKeys; ++i) {
        for (const auto& seed : this->hashSeeds) {
            uint64_t index = compute_hash(keys[i], seed) % (this->filter.size());
            this->filter[index] = 1;
        }
    }
}

template<typename T>
bool BloomFilter<T>::contains(uint64_t key) {
    for (const auto& seed : this->hashSeeds) {
        uint64_t index = compute_hash(key, seed) % (this->filter.size());
        if (this->filter[index] == 0) {
            return false;
        }
    }
    return true;
}

template<typename T>
vector<T> BloomFilter<T>::getFilter() {
    return this->filter;
}


template<typename T>
int BloomFilter<T>::getMemoryOccupied() {
    return  this->filter.size();
}

template class BloomFilter<bool>;
