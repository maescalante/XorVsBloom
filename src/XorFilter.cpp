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
#include <future>
#include <thread>


using namespace std;

/*
Check if a given key is present in the filter
*/
template<typename FingerprintType>
bool XorFilter<FingerprintType>::contains(uint64_t key) {
    uint32_t h0 = compute_hash(key, this->h0Seed, 0, this->filter.size());
    uint32_t h1 = compute_hash(key, this->h1Seed, 1, this->filter.size());
    uint32_t h2 = compute_hash(key, this->h2Seed, 2, this->filter.size());

    FingerprintType fp  = compute_fingerprint<FingerprintType>(key, this->fingerprintSeed);
   
    return fp == (this->filter[h0] ^ this->filter[h1] ^ this->filter[h2]);
}

/*
Peeling operation to construct the xor filter
*/
template<typename FingerprintType>
stack<pair<uint64_t, uint32_t>> XorFilter<FingerprintType>::peel(vector<uint64_t>& keys, vector<uint64_t>& hashFunctionSeeds) {
    int c = floor(1.23 * keys.size()) + 32;
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
                H[hashValue].erase(remove(H[hashValue].begin(), H[hashValue].end(), x), H[hashValue].end());
                if (H[hashValue].size() == 1){
                    Q.push(hashValue);
                }
            }
        }
    }

    return stack;
}

/*
Assign all entries to the slots found in the peeling step 
*/
template<typename FingerprintType>
void XorFilter<FingerprintType>::assign(stack<pair<uint64_t, uint32_t>>& stack) {
    this -> fingerprintSeed = generateRandomUInt64();
    int c = floor(1.23 * stack.size()) + 32;

    while (!stack.empty()) {
        std::pair<uint64_t, uint32_t> element = stack.top();
        uint64_t x = element.first;
        uint64_t i = element.second;
        stack.pop(); 
        this->filter[i] = 0;
        uint32_t h0 = compute_hash(x, this->h0Seed, 0, c);
        uint32_t h1 = compute_hash(x, this->h1Seed, 1, c);
        uint32_t h2 = compute_hash(x, this->h2Seed, 2, c);
        FingerprintType fp = compute_fingerprint<FingerprintType>(x, this->fingerprintSeed);
        this->filter[i] = fp ^ filter[h0] ^ filter[h1] ^ filter[h2];
    }
}

/*
Build Xor filter
*/
template<typename FingerprintType>
void XorFilter<FingerprintType>::build(vector<uint64_t>& keys) {
    
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
            this -> buildFailures = retryCount;
        }else {
            retryCount++;
            cout << "failed building, retrying..."<< endl;
        }
    }
    this->filter.resize(floor(1.23 * keys.size()) + 32);

    assign(stack);
}

/*
return filter
*/
template<typename FingerprintType>
vector<FingerprintType> XorFilter<FingerprintType>::getFilter() {
    return this->filter;
}

/*
Get memory occupied by the filter
*/
template<typename FingerprintType>
int XorFilter<FingerprintType>::getMemoryOccupied() {
    return  this->filter.size() * sizeof(FingerprintType);
}

/*
Reset filter 
*/
template<typename FingerprintType>
void XorFilter<FingerprintType>::resetFilter() {
    this->filter.clear();
}


template class XorFilter<uint8_t>;
template class XorFilter<uint16_t>;
