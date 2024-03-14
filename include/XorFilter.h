#ifndef XOR_FILTER_H
#define XOR_FILTER_H

#include <iostream>
#include <vector>
#include <stack>
#include <functional>
#include <utility>



using namespace std;

class XorFilter {

private:

    stack<pair<uint64_t, int>> peel(vector<uint64_t> keys, vector<uint64_t> hashFunctionSeeds);
    void build(vector<uint64_t> keys);
    void assign(stack<pair<uint64_t, int>> stack);

public:
    vector<uint64_t> B;
    XorFilter(vector<uint64_t> keys);

    bool contains(const int64_t& key);
    uint32_t h0Seed;
    uint32_t h1Seed;
    uint32_t h2Seed;
    uint64_t fingerprintSeed;
    int filterSize;
};

#endif