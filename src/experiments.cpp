#include <iostream>
#include <random>
#include <vector>
#include <unordered_map>
#include "dataUtils.cpp"
#include "statistics.cpp"
#include "Filter.h"
#include "helpers.cpp"


using namespace std;


inline void printResults(float fpr,float time,float empty,float mem, float timePerKey, float memPerkey){
    cout << "filter construction time: " << time << " ms" << endl;
    cout << "false positive rate is: " << fpr << "%\n";
    cout << "Percent of empty entries in the filter: " << fpr << "%\n";
    cout << "filter size: " << mem << " bytes"<< endl; 
    cout << "time per key: " << timePerKey << " ms"<< endl; 
    cout << "memory per key: " << memPerkey << " bytes"<< endl; 
    cout << endl;
}


template<typename T>
inline void runExperiment(Filter<T> *filter, size_t totalItems, size_t iterations, double fractionKeysNotInSet, bool print = false){
    vector<float> fprs;
    vector<float> constructionTimes;
    vector<float> emptyEntries;
    vector<float> memoryOccupied;

    size_t i = 0;
    while (i < iterations) {

        // Insert items to this xor filter
        vector<uint64_t> data = generateUniqueRandomNumbers(0, totalItems*100, totalItems);

        // Measure the execution time and construct XorFilter object
        double timeTaken = measureExecutionTime([&]() {
            filter->build(data);
        });

        constructionTimes.push_back(timeTaken);
        
        size_t intersectionSize = data.size() * (1-fractionKeysNotInSet);
        replaceNumbers(data, fractionKeysNotInSet*100);

        // Check non-existing items, a few false positives expected
        size_t found = 0;
        for (uint64_t i = 0; i < totalItems; i++) {
            if (filter->contains(data[i]) == true) {
                found++;
            }
        }

        fprs.push_back(fpr(found, intersectionSize, data.size()));


        size_t emptyCount = 0 ;
        for (size_t i = 0; i < filter->getFilter().size(); ++i) {
            if (filter->getFilter()[i] == 0){
                emptyCount++;
            }
        }

        emptyEntries.push_back(emptyCount/filter->getFilter().size() * 100);

        // Calculate memory occupied by the elements
        size_t elementSize = filter->getFilter().size() * sizeof(filter->getFilter()[0]);
        memoryOccupied.push_back(elementSize);

        i++;
    }

    float fpr = calculateAverage(fprs);
    float time = calculateAverage(constructionTimes);
    float empty = calculateAverage(emptyEntries);
    float mem = calculateAverage(memoryOccupied); 
    float timePerKey = time/totalItems;
    float memPerKey = mem/totalItems;

    if(print) {
        printResults(fpr, time, empty, mem, timePerKey, memPerKey);
    }

}


