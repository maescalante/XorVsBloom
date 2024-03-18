#include <iostream>
#include <random>
#include <vector>
#include <unordered_map>
#include "dataUtils.cpp"
#include "XorFilter.h"

using namespace std;
template<typename FingerprintType>
inline void fpr(size_t size, int iterations){
    unordered_map<string, vector<float>> fprs;

    int i = 0;
    while (i < iterations) {
        /*
        vector<uint64_t> skewed = generateSkewedData(size, 0.7);
        vector<uint64_t> random = generateUniformData(size, 0, size*1000);
        vector<uint64_t> zipf = generateZipfData(size, size/2, 0.2);


        XorFilter<FingerprintType> xorFilterSkewed(skewed);
        XorFilter<FingerprintType> xorFilterRandom(random);
        XorFilter<FingerprintType> xorFilterZipf(zipf);


        size_t falseQueriesSkewed = 0;
        size_t falseQueriesRand = 0;
        size_t falseQueriesZipf = 0;


        for (uint64_t i = ; i < 2 * total_items; i++) {
            if (xorFilterTest.contains(i) == true) {
                false_queries++;
            }
            total_queries++;
        }


        
        //handle duplicates dude



   */

        i++;
    }
 

}