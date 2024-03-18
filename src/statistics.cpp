#include <iostream>
#include <chrono>

using namespace std;

inline float fpr (size_t numFound, size_t intersectionSize, size_t dataSize){
    return (numFound- intersectionSize)/ static_cast<double>(dataSize - intersectionSize);
}


template<typename Func>
double measureExecutionTime(Func func) {
    auto start = chrono::high_resolution_clock::now(); // Get the current time before executing the function
    func(); // Call the provided function
    auto end = chrono::high_resolution_clock::now(); // Get the current time after executing the function

    // Calculate the duration in milliseconds
    chrono::duration<double, milli> duration = end - start;
    return duration.count(); // Return the duration in millisecons
}