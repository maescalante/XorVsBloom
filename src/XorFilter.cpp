#include "XorFilter.h"
#include <cmath>
#include <functional> 
#include <queue>
#include <stack>
#include <algorithm>
#include <utility>
#include <random>
#include <unordered_map>




using namespace std;
using Pair = std::pair<int, int>;

XorFilter::XorFilter(vector<uint64_t> keys) {
    build(keys);
}


uint64_t hashFunction(uint64_t key, uint64_t seed){
    uint64_t h = key + seed;
    h ^= h >> 33;
    h *= UINT64_C(0xff51afd7ed558ccd);
    h ^= h >> 33;
    h *= UINT64_C(0xc4ceb9fe1a85ec53);
    h ^= h >> 33;
    return h;
}


uint32_t compute_fingerprint(uint64_t key, uint64_t seed){
    seed <<= 32;
    uint64_t h = hashFunction(key, seed);
    return  (uint32_t)(h ^ (h >> 32));
}

uint32_t compute_hash(uint64_t key, uint64_t seed, int index, int c){
    uint64_t h = hashFunction(key, seed);
    h = (uint32_t) h;
    uint32_t rangeMin = index * (c/3);
    uint32_t rangeMax = (c/3 * (index +1)) -1;
    if (index == 2) {
        rangeMax += c % 3;
    }
    h = h % (rangeMax - rangeMin + 1) + rangeMin;
    
    return (uint32_t)h;
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
            this->h2Seed = seed2;
        }else{
            retryCount++;
            if (retryCount > 10){
                while (!stack.empty()) {
                    // Get the top element of the stack
                    Pair currentPair = stack.top();
                    // Print the pair's first and second elements
                    std::cout << "(" << currentPair.first << ", " << currentPair.second << ")" << std::endl;
                    // Remove the top element from the temporary stack
                    stack.pop();
                }
                throw std::runtime_error("failed to build");
            }
            cout << "failed building, retrying..."<< endl;
        }
    }
    cout << "stack size: " <<stack.size()<< endl;
    this->B.resize(floor(1.23 * keys.size()) + 32);

    assign(stack);
}