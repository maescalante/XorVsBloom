#include <iostream>
#include "XorFilter.h"
#include <vector>
#include "BloomFilter.h"
#include "experiments.cpp"
#include "Filter.h"

using namespace std;

int main() {
    cout << "XOR FILTER! " << endl;
    size_t iterations = 3;
    runForFractionKeys(iterations, 1000000);
}
