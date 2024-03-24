#include <random>
#include <iostream>
#include <cstdint>

using namespace std;

inline uint64_t generateRandomUInt64() {
    // Use a random device as a source of entropy
    random_device rd;

    // Use a random number engine and seed it with entropy from random device
    mt19937_64 gen(rd());

    // Define the range of the random numbers
    uniform_int_distribution<uint64_t> dis;

    // Generate a random uint64_t number
    return dis(gen);
}

inline float calculateAverage(const vector<float>& vec) {
    float sum = 0.0f;
    for (float value : vec) {
        sum += value;
    }
    return sum / vec.size();
}
