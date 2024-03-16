#include "XorFilter.h"
#include <cmath>
#include <queue>
#include <stack>
#include <algorithm>
#include <utility>
#include <unordered_map>
#include "helpers.cpp"
#include "hashUtils.cpp"
#include <cstdint>


using namespace std;
using Pair = std::pair<int, int>;

XorFilter::XorFilter(vector<uint64_t> keys) {
    build(keys);
}


bool XorFilter::contains(const uint64_t key) {
    uint32_t h0 = compute_hash(key, this->h0Seed, 0, this->B.size());
    uint32_t h1 = compute_hash(key, this->h1Seed, 1, this->B.size());
    uint32_t h2 = compute_hash(key, this->h2Seed, 2, this->B.size());

    uint32_t fp  = compute_fingerprint(key, this->fingerprintSeed);
   
    return fp == (this->B[h0] ^ this->B[h1] ^ this->B[h2]);
}



stack<pair<uint64_t, uint32_t>> XorFilter::peel(vector<uint64_t> keys, vector<uint64_t> hashFunctionSeeds) {
    int c = floor(1.23 * keys.size()) + 32;
   //vector<vector<uint32_t>> H(c);
    unordered_map<uint32_t, vector<uint64_t>> H;

    for (auto x = keys.begin(); x != keys.end(); ++x) {
        for (size_t i = 0; i < hashFunctionSeeds.size(); ++i) {
            uint32_t hashValue = compute_hash(*x, hashFunctionSeeds[i], i, c);
            if (H.find(hashValue) != H.end()){
                H[hashValue].push_back(*x);
            } else {
                vector<uint64_t> vec = {*x};
                H[hashValue] = vec;
            }
        }
    }

    
    queue<int> Q;
    for (const auto& pair : H) {
        if (pair.second.size() == 1) {
            Q.push(pair.first);
        }
    }

    stack<pair<uint64_t, uint32_t>> stack;
    while(!Q.empty()){
        uint32_t i = Q.front();
        Q.pop();

        if (H[i].size() == 1){
            uint64_t x = H[i][0];
            stack.push(make_pair(x,i));
            for (size_t j = 0; j < hashFunctionSeeds.size(); ++j) {
                uint32_t hashValue = compute_hash(x, hashFunctionSeeds[j], j, c);
                H[hashValue].erase(remove(H[hashValue].begin(), H[hashValue].end(), x), H[hashValue].end()); // careful you are delteng all occurrences
                if (H[hashValue].size() == 1){
                    Q.push(hashValue);
                }
            }
        }
    }

    return stack;
}


void XorFilter::assign(stack<pair<uint64_t, uint32_t>> stack) {
    this -> fingerprintSeed = generateRandomUInt64();
    int c = floor(1.23 * stack.size()) + 32;

    while (!stack.empty()) {
        std::pair<uint64_t, uint32_t> element = stack.top();
        uint64_t x = element.first;
        uint64_t i = element.second;
        stack.pop(); // Remove the top element from the stack
        this->B[i] = 0;
        uint32_t h0 = compute_hash(x, this->h0Seed, 0, c);
        uint32_t h1 = compute_hash(x, this->h1Seed, 1, c);
        uint32_t h2 = compute_hash(x, this->h2Seed, 2, c);
        uint32_t fp = compute_fingerprint(x, this->fingerprintSeed);
        // so here i assume h0 was i but it might be other so ??

        this->B[i] = fp ^ B[h0] ^ B[h1] ^ B[h2];
    }
}


void XorFilter::build(vector<uint64_t> keys) {
    
    bool flag = false;
    stack<pair<uint64_t, uint32_t>> stack;
    vector<uint64_t> seeds;
    int retryCount = 0;
    while(!flag){
        uint64_t seed0 = generateRandomUInt64();
        uint64_t seed1 = generateRandomUInt64();
        uint64_t seed2 = generateRandomUInt64();

        seeds = {seed0, seed1, seed2};

        stack = peel(keys, seeds);

        if (stack.size() == keys.size()){
            flag = true; 
            this->h0Seed = seed0;
            this->h1Seed = seed1;
            this->h2Seed = seed2;\
            this -> buildFailures = retryCount;
        }else {
            retryCount++;
            cout << "failed building, retrying..."<< endl;
        }
    }
    this->B.resize(floor(1.23 * keys.size()) + 32);

    assign(stack);
}