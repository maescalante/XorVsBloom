#include <iostream>
#include "XorFilter.h"
#include <vector>
#include <assert.h>


using namespace std;

int main() {
    cout << "XOR FILTER! " << endl;

    vector<uint64_t> keys = {2, 4, 6, 12};
    XorFilter* xorFilter = new XorFilter(keys);
    cout << xorFilter->contains(2) << endl;
    cout << xorFilter->contains(3) << endl;


    size_t total_items = 0;

    
    vector<uint64_t> keysTest;

    // Insert items to this cuckoo filter
    size_t num_inserted = 0;
    for (size_t i = 0; i < total_items; i++, num_inserted++) {
        keysTest.push_back(i);
    }
    XorFilter* xorFilterTest = new XorFilter(keysTest);

    
    cout << "tests"<< endl;

    // Check if previously inserted items are in the filter, expected
    // true for all items
    size_t total_queries = 0;
    size_t true_queries = 0;
    for (size_t i = 0; i < num_inserted; i++) {
          if (xorFilterTest->contains(i) == 1) {
        true_queries++;
        }
        total_queries++;
    }

    cout << "percent of true positives: "
            << 100.0 * true_queries / total_queries << "%\n";

    // Check non-existing items, a few false positives expected
    total_queries = 0;
    size_t false_queries = 0;
    for (size_t i = total_items; i < 2 * total_items; i++) {
        if (xorFilterTest->contains(i) == 1) {
        false_queries++;
        }
        total_queries++;
    }

    // Output the measured false positive rate
    cout << "false positive rate is: "
                << 100.0 * false_queries / total_queries << "%\n";



    
    delete xorFilter;
}
