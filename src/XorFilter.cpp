#include "XorFilter.h"
#include <cmath>
#include <functional> 
#include <queue>
#include <stack>
#include <algorithm>
#include <utility>
#include <random>




using namespace std;
using Pair = std::pair<int, int>;

XorFilter::XorFilter(vector<uint64_t> keys) {
    build(keys);
}


uint64_t hashFunction(int64_t key, int64_t seed){
    uint64_t h = key + seed;
    h ^= h >> 33;
    h *= UINT64_C(0xff51afd7ed558ccd);
    h ^= h >> 33;
    h *= UINT64_C(0xc4ceb9fe1a85ec53);
    h ^= h >> 33;
    return h;
}


uint32_t compute_fingerprint(int64_t key, int64_t seed){
    uint64_t h = hashFunction(key, seed);
    return  (uint32_t)(h ^ (h >> 32));
}

uint32_t compute_hash(int64_t key, int64_t seed, int index, int c){
    uint64_t h = hashFunction(key, seed);
    h = (uint32_t) h;
    uint32_t rangeMax = (c/3 * (index +1)) -1;
    return  (uint32_t) (((uint64_t) h * rangeMax) >> 32);
}

bool XorFilter::contains(const int64_t& key) {
    cout << "seeds " << this->h0Seed << " " << this->h1Seed << " " << this->h2Seed << " " << this->fingerprintSeed<< endl; 

    uint32_t h0 = compute_hash(key, this->h0Seed, 0, this->filterSize);
    uint32_t h1 = compute_hash(key, this->h1Seed, 1, this->filterSize);
    uint32_t h2 = compute_hash(key, this->h2Seed, 2, this->filterSize);

    uint32_t fp  = compute_fingerprint(key, this->fingerprintSeed);
    cout << "key " << key << endl;
    cout << "h0 "<< h0 << endl;
    cout << "h1 "<< h1 << endl;
    cout << "h2 "<< h2 << endl;
    cout << "fp "<< fp << endl;     

    for (int i = 0; i < this->B.size(); ++i) {
        std::cout << this->B[i] << " ";
    }
    std::cout << std::endl;

    cout << "xorContains" << (this->B[h0] ^ this->B[h1] ^ this->B[h2]) << endl;
    return fp == (this->B[h0] ^ this->B[h1] ^ this->B[h2]);
}



stack<pair<uint64_t, int>> XorFilter::peel(vector<uint64_t> keys, vector<uint64_t> hashFunctionSeeds) {
    int c = floor(1.23 * keys.size()) + 32;
    this->filterSize = c;
    vector<vector<uint32_t>> H(c);
    for (auto x = keys.begin(); x != keys.end(); ++x) {
        for (size_t i = 0; i < hashFunctionSeeds.size(); ++i) {
            uint32_t hashValue = compute_hash(*x, hashFunctionSeeds[i], i, c);
            H[hashValue].push_back(*x);
        }
    }

    
    queue<int> Q;

    for (size_t i = 0; i < H.size() ; i++) {
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



void XorFilter::assign(stack<pair<uint64_t, int>> stack) {
    this -> fingerprintSeed = generateRandomUInt64();
    int c = floor(1.23 * stack.size()) + 32;

    while (!stack.empty()) {
        std::pair<int, char> element = stack.top();
        int64_t x = element.first;
        int64_t i = element.second;
        stack.pop(); // Remove the top element from the stack
        this->B[i] = 0;
        uint32_t h0 = compute_hash(x, this->h0Seed, 0, c);
        uint32_t h1 = compute_hash(x, this->h1Seed, 1, c);
        uint32_t h2 = compute_hash(x, this->h2Seed, 2, c);
        uint32_t fp = compute_fingerprint(x, this->fingerprintSeed);
        cout << "i " << i << endl;

        cout << "x " << x << endl;
        cout << "h0 "<< h0 << endl;
        cout << "h1 "<< h1 << endl;
        cout << "h2 "<< h2 << endl;
        cout << "fp "<< fp << endl;

        cout << "orgXOR" << (fp ^ B[h0] ^ B[h1] ^ B[h2]) << endl; 

        this->B[i] = fp ^ B[h0] ^ B[h1] ^ B[h2];
    }
}


void XorFilter::build(vector<uint64_t> keys) {
    
    bool flag = false;
    stack<pair<uint64_t, int>> stack;
    vector<uint64_t> seeds;
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
        }
    }
   this->B.resize(floor(1.23 * keys.size()) + 32);

    assign(stack);
}