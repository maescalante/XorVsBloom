#include <iostream>
#include <random>
#include <vector>
#include <cstdint>
#include <cmath>
#include <unordered_set>
#include <algorithm>



using namespace std;

inline vector<uint64_t> generateUniqueRandomNumbers(uint64_t min, uint64_t max, size_t count) {

    vector<uint64_t> uniqueNumbers;
    unordered_set<uint64_t> seenNumbers; // Hash set to keep track of generated numbers
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint64_t> distribution(min, max);

    while (uniqueNumbers.size() < count) {
        uint64_t num = distribution(gen);
        if (seenNumbers.find(num) == seenNumbers.end()) {
            uniqueNumbers.push_back(num);
            seenNumbers.insert(num);
        }
    }

    return uniqueNumbers;
}


inline void deleteRandomPercentage(vector<uint64_t>& numbers, double percentage) {
   if (percentage <= 0.0 || percentage >= 100.0)
        return; // No elements to delete or delete all elements

    size_t numToDelete = static_cast<size_t>(numbers.size() * (percentage / 100.0));
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<size_t> distribution(0, numbers.size() - 1);

    for (size_t i = 0; i < numToDelete; ++i) {
        size_t indexToDelete = distribution(gen);
        numbers[indexToDelete] = numbers.back(); // Replace element to delete with the last element
        numbers.pop_back(); // Remove the last element
    }
}



inline void replaceNumbers(std::vector<uint64_t>& numbers, double percentage) {
    auto maxIterator = max_element(numbers.begin(), numbers.end());
    int previousSize = numbers.size();

    uint64_t maxValue = *maxIterator;

    deleteRandomPercentage(numbers, percentage);
    int neededValues = previousSize - numbers.size();
    vector<uint64_t> newValues = generateUniqueRandomNumbers(maxValue+1, maxValue + neededValues+10000, neededValues); 
    numbers.insert(numbers.end(), newValues.begin(), newValues.end());
}