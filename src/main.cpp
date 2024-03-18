#include <iostream>
#include "XorFilter.h"
#include <vector>
#include <cstdint>
#include "dataUtils.cpp"
#include "statistics.cpp"

using namespace std;

int main() {
    cout << "XOR FILTER! " << endl;

    size_t total_items = 1000000;

    // Insert items to this xor filter
    vector<uint64_t> data = generateUniqueRandomNumbers(0, total_items*100, total_items);

    // Measure the execution time and construct XorFilter object
    XorFilter<uint8_t> xorFilterTest;

    double timeTaken = measureExecutionTime([&]() {
        xorFilterTest.build(data);
    });

    size_t total_queries = 0;
    size_t trueQ = 0;
    for (uint64_t i = 0; i < total_items; i++) {
        if (xorFilterTest.contains(data[i]) == true) {
            trueQ++;
        }
        total_queries++;
    }



    cout << "filter construction time: " <<timeTaken<< " ms" << endl;
    cout << "failure while constructing filter: " << xorFilterTest.buildFailures << endl;
    
    size_t intersectionSize = data.size()  * (1-0.25);
    replaceNumbers(data, 25);

    // Check non-existing items, a few false positives expected
    size_t found = 0;
    for (uint64_t i = 0; i < total_items; i++) {
        if (xorFilterTest.contains(data[i]) == true) {
            found++;
        }
    }

    for (const auto& byte : xorFilterTest.B) {
      //  std::cout << "Byte " << static_cast<int>(byte) << " in binary: ";
        for (int i = 7; i >= 0; --i) {
            cout << ((byte >> i) & 1);
        }
        cout << std::endl;
    }

    

    size_t emptyCount = 0 ;
    for (size_t i = 0; i < xorFilterTest.B.size(); ++i) {
        //cout << static_cast<int>(xorFilterTest.B[i]) << endl; 
        if (xorFilterTest.B[i] == 0){
            emptyCount++;
        }
    }
    cout << emptyCount << endl;

    cout << "false positive rate is: " 
     << fpr(found, intersectionSize, data.size()) << "%\n";
    cout << (trueQ==total_queries) <<endl;   

}
