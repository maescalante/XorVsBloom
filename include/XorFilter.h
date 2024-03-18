#ifndef XOR_FILTER_H
#define XOR_FILTER_H

#include <iostream>
#include <vector>
#include <stack>
#include <functional>
#include <utility>



using namespace std;
template<typename FingerprintType>
class XorFilter {

private:

    stack<pair<uint64_t, uint32_t>> peel(vector<uint64_t>& keys, vector<uint64_t>& hashFunctionSeeds);
    void assign(stack<pair<uint64_t, uint32_t>>& stack);

public:
    vector<FingerprintType> B;
    void build(vector<uint64_t>& keys);

    bool contains(const uint64_t key);
    uint64_t h0Seed;
    uint64_t h1Seed;
    uint64_t h2Seed;
    uint64_t fingerprintSeed;
    int buildFailures;

};

#endif