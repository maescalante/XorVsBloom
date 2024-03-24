#include <iostream>
#include "XorFilter.h"
#include <vector>
#include "BloomFilter.h"
#include "experiments.cpp"
#include "Filter.h"

using namespace std;

int main() {
    cout << "XOR FILTER! " << endl;
    size_t totalItems = 1000000;
    size_t iterations = 1;
    Filter<bool>* bloom =  new BloomFilter<bool>(8);
    Filter<uint8_t>* xorFilter = new XorFilter<uint8_t>();


    runExperiment(bloom, totalItems, iterations, 0.25, true);
    runExperiment(xorFilter, totalItems, iterations, 0.25, true);

    delete bloom;
    delete xorFilter;

}
