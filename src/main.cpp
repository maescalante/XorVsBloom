#include <iostream>
#include "XorFilter.h"
#include <vector>
#include <assert.h>


using namespace std;

int main() {
    cout << "XOR FILTER! " << endl;

    vector<uint64_t> keys = {2,4, 6, 12};
    XorFilter* xorFilter = new XorFilter(keys);
    cout << xorFilter->contains(2) << endl;
    cout << xorFilter->contains(3) << endl;
    cout << xorFilter->contains(6) << endl;

    delete xorFilter;


    size_t total_items = 1000000;

    
    vector<uint64_t> keysTest;

    // Insert items to this xor filter
    for (uint64_t i = 0; i < total_items; i++) {
        keysTest.push_back(i);
    }
    XorFilter* xorFilterTest = new XorFilter(keysTest);

    
    cout << "tests"<< endl;

    // Check if previously inserted items are in the filter, expected
    // true for all items
    size_t total_queries = 0;
    size_t true_queries = 0;
    for (uint64_t i = 0; i < total_items; i++) {
        if (xorFilterTest->contains(i) == true) {
            true_queries++;
        }
        total_queries++;
    }

    cout << "percent of true positives: " 
     << static_cast<float>(100.0 * true_queries) / total_queries << "%\n";
    // Check non-existing items, a few false positives expected
    total_queries = 0;
    size_t false_queries = 0;
    for (uint64_t i = total_items; i < 2 * total_items; i++) {
        if (xorFilterTest->contains(i) == true) {
            false_queries++;
        }
        total_queries++;
    }


    cout << "false positive rate is: " 
     << static_cast<float>(100.0 * false_queries) / total_queries << "%\n";

    
    delete xorFilterTest;
}
