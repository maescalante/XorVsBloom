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
    void assign(stack<pair<uint64_t, int>> stack, vector<uint64_t> hashFunctionSeeds);

public:
    uint64_t* B;
    XorFilter(vector<uint64_t> keys);

    bool contains(const int64_t& key);
    uint64_t h1Seed;
    uint64_t h2Seed;
    uint64_t h3Seed;
    uint64_t fingerprintFunctionSeed;
};

#endif