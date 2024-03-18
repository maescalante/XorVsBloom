#include <iostream>
#include "XorFilter.h"
#include <vector>
#include <cstdint>
#include "dataUtils.cpp"
#include "statistics.cpp"

using namespace std;

int main() {
    cout << "XOR FILTER! " << endl;

    size_t total_items = 100000;

    // Insert items to this xor filter
    vector<uint64_t> data = generateUniqueRandomNumbers(0, total_items*100, total_items);

    // Measure the execution time and construct XorFilter object
    XorFilter<uint8_t> xorFilterTest;

    double timeTaken = measureExecutionTime([&]() {
        xorFilterTest.build(data);
    });

    cout<< timeTaken<< endl;
    
    cout << "tests"<< endl;

    size_t intersectionSize = data.size()  * (1-0.25);
    replaceNumbers(data, 25);


    // Check non-existing items, a few false positives expected
    size_t total_queries = 0;
    size_t found = 0;
    for (uint64_t i = 0; i < total_items; i++) {
        if (xorFilterTest.contains(data[i]) == true) {
            found++;
        }
        total_queries++;
    }

    cout << "false positive rate is: " 
     << fpr(found, intersectionSize, data.size()) << "%\n";

}
