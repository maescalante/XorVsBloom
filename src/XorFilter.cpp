#include "XorFilter.h"
#include <cmath>
#include <functional> 
#include <queue>
#include <stack>
#include <algorithm>
#include <utility>
#include "xxhash.h"
#include <random>




using namespace std;
using Pair = std::pair<int, int>;

XorFilter::XorFilter(vector<uint64_t> keys) {
    build(keys);
}


bool XorFilter::contains(const int64_t& key) {
  uint64_t fp = XXH64(&(key), sizeof(key), this->fingerprintFunctionSeed);
  uint64_t h1 = XXH64(&(key), sizeof(key), this->h1Seed);
  uint64_t h2 = XXH64(&(key), sizeof(key), this->h2Seed);
  uint64_t h3 = XXH64(&(key), sizeof(key), this->h3Seed);

  return fp == (this->B[h1] ^ this->B[h2] ^ this->B[h3]);
}

uint64_t compute_hash(int64_t key, int64_t seed, int index, int c){
    uint32_t hashValue = XXH32(&(key), sizeof(key), seed);
    uint32_t rangeMax = (c/3 * (index +1)) -1;
    return  (uint32_t) (((uint64_t) hashValue * rangeMax) >> 32);
}


stack<pair<uint64_t, int>> XorFilter::peel(vector<uint64_t> keys, vector<uint64_t> hashFunctionSeeds) {
    int c = floor(1.23 * keys.size()) + 32;
    vector<uint64_t> H[c];
    for (auto x = keys.begin(); x != keys.end(); ++x) {
        for (size_t i = 0; i < hashFunctionSeeds.size(); ++i) {
            uint64_t hashValue = compute_hash(*x, hashFunctionSeeds[i], i, c);
            H[hashValue].push_back(*x);
        }
    }
    
    queue<int> Q;

    for (size_t i = 0; i < H->size() ; i++) {
        if (H[i].size() == 1) {
            Q.push(i);
        }
    }

    stack<pair<uint64_t, int>> stack;
    while(!Q.empty()){
        int i = Q.front();
        Q.pop();

        if (H[i].size() == 1){
            uint64_t x = H[i][0];
            stack.push(make_pair(x,i));
            for (size_t i = 0; i < hashFunctionSeeds.size(); ++i) {
                uint64_t hashValue = compute_hash(x, hashFunctionSeeds[i], i, c);
                H[hashValue].erase(remove(H[hashValue].begin(), H[hashValue].end(), x), H[hashValue].end()); 
                if (H[hashValue].size() == 1){
                    Q.push(hashValue);
                }
            }
        }
    }

    return stack;
}

uint64_t generateRandomUInt64() {
    // Use a random device as a source of entropy
    std::random_device rd;

    // Use a random number engine and seed it with entropy from random device
    std::mt19937_64 gen(rd());

    // Define the range of the random numbers
    std::uniform_int_distribution<uint64_t> dis;

    // Generate a random uint64_t number
    return dis(gen);
}

void XorFilter::assign(stack<pair<uint64_t, int>> stack, vector<uint64_t> hashFunctionSeeds) {
    this -> fingerprintFunctionSeed = generateRandomUInt64();
    int c = floor(1.23 * stack.size()) + 32;

    while (!stack.empty()) {
        std::pair<int, char> element = stack.top();
        int64_t x = element.first;
        int64_t i = element.second;
        stack.pop(); // Remove the top element from the stack
        this->B[i] = 0;
        int64_t h0 = compute_hash(x, hashFunctionSeeds[0], 0, c);
        int64_t h1 = compute_hash(x, hashFunctionSeeds[1], 1, c);
        int64_t h2 = compute_hash(x, hashFunctionSeeds[2], 2, c);

        this->B[i] = XXH64(&(x), sizeof(x), fingerprintFunctionSeed) ^ B[h0] ^ B[h1] ^ B[h2];
    }
}


void XorFilter::build(vector<uint64_t> keys) {
    
    bool flag = false;
    stack<pair<uint64_t, int>> stack;
    vector<uint64_t> seeds;
    while(!flag){
        uint64_t seed1 = generateRandomUInt64();
        uint64_t seed2 = generateRandomUInt64();
        uint64_t seed3 = generateRandomUInt64();

        seeds = {seed1, seed2, seed3};

        stack = peel(keys, seeds);

        if (stack.size() == keys.size()){
            flag = true; 
            this->h1Seed = seed1;
            this->h2Seed = seed2;
            this->h3Seed = seed3;
        }
    }
    this->B = new uint64_t[static_cast<size_t>(std::floor(1.23 * keys.size()) + 32)];

    assign(stack, seeds);
}