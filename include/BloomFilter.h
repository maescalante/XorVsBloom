#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <iostream>
#include <vector>
#include "Filter.h"

using namespace std;

template<typename T>
class BloomFilter: public Filter<T> {
private:
    vector<uint64_t> hashSeeds;
    size_t bitsPerEntry;
    vector<T> filter;

public:

    BloomFilter(size_t bitsPerEntry);
    void build(vector<uint64_t>& keys);
    bool contains(uint64_t key) override;
    vector<T> getFilter() override;
    int getMemoryOccupied() override;

};

#endif