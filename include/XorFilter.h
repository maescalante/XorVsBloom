#ifndef XOR_FILTER_H
#define XOR_FILTER_H

#include <iostream>
#include <vector>
#include <stack>
#include <functional>
#include <utility>
#include "Filter.h"


using namespace std;
template<typename FingerprintType>
class XorFilter: public Filter<FingerprintType> {

private:

    stack<pair<uint64_t, uint32_t>> peel(vector<uint64_t>& keys, vector<uint64_t>& hashFunctionSeeds);
    void assign(stack<pair<uint64_t, uint32_t>>& stack);
    uint64_t h0Seed;
    uint64_t h1Seed;
    uint64_t h2Seed;
    uint64_t fingerprintSeed;
    vector<FingerprintType> filter;

public:
    void build(vector<uint64_t>& keys) override;
    bool contains(uint64_t key) override;
    vector<FingerprintType> getFilter() override;
    int getMemoryOccupied() override;
    int buildFailures;

};

#endif