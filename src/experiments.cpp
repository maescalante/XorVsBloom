#include <iostream>
#include <random>
#include <vector>
#include <unordered_map>
#include "dataUtils.cpp"
#include "statistics.cpp"
#include "Filter.h"
#include "helpers.cpp"
#include <string>
#include <fstream>
#include "XorFilter.h"
#include "BloomFilter.h"


using namespace std;
const vector<string> resultFilenames = { "fpr", "constructionTime", "memory", "timePerKey", "memoryPerKey"};

inline void printResults(float fpr,float time,float mem, float timePerKey, float memPerkey){
    cout << "filter construction time: " << time << " ms" << endl;
    cout << "false positive rate is: " << fpr << "%\n";
    cout << "filter size: " << mem << " bytes"<< endl; 
    cout << "time per key: " << timePerKey << " ms"<< endl; 
    cout << "memory per key: " << memPerkey << " bytes"<< endl; 
    cout << endl;
}

template<typename T>
inline void runExperiment(vector<string> filenames, Filter<T> *filter, size_t totalItems, size_t iterations, double fractionKeysNotInSet, bool print = false){
    vector<float> fprs;
    vector<float> constructionTimes;
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

        // Calculate memory occupied by the elements
        memoryOccupied.push_back(filter->getMemoryOccupied());
        i++;
    }

    float fpr = calculateAverage(fprs);
    float time = calculateAverage(constructionTimes);
    float mem = calculateAverage(memoryOccupied); 
    float timePerKey = time/totalItems;
    float memPerKey = mem/totalItems;

    vector<float> results = {fpr, time, mem, timePerKey, memPerKey};

    if(print) {
        printResults(fpr, time, mem, timePerKey, memPerKey);
    }
    
    writeToCSV(filenames[0], to_string(fpr));
    writeToCSV(filenames[1], to_string(time));
    writeToCSV(filenames[2], to_string(mem));
    writeToCSV(filenames[3], to_string(timePerKey));
    writeToCSV(filenames[4], to_string(memPerKey));
}





inline void runForAll(vector<string> filenames, size_t keyNum, size_t iterations, double fractionKeysNotInSet, bool print=false){
    Filter<bool>* bloom8 =  new BloomFilter<bool>(8);
    cout << "Running for Bloom8, # of keys: " << keyNum << ", fraction of keys not in set: " << fractionKeysNotInSet << endl;
    runExperiment(filenames, bloom8, keyNum, iterations, fractionKeysNotInSet, print = false);

    Filter<bool>* bloom10 =  new BloomFilter<bool>(10);
    cout << "Running for Bloom10, # of keys: " << keyNum << ", fraction of keys not in set: " << fractionKeysNotInSet << endl;
    runExperiment(filenames, bloom10, keyNum, iterations, fractionKeysNotInSet, print = false);

    Filter<bool>* bloom12 =  new BloomFilter<bool>(12);
    cout << "Running for Bloom12, # of keys: " << keyNum << ", fraction of keys not in set: " << fractionKeysNotInSet << endl;
    runExperiment(filenames, bloom12, keyNum, iterations, fractionKeysNotInSet, print = false);

    Filter<bool>* bloom16 =  new BloomFilter<bool>(16);
    cout << "Running for Bloom16, # of keys: " << keyNum << ", fraction of keys not in set: " << fractionKeysNotInSet << endl;
    runExperiment(filenames, bloom16, keyNum, iterations, fractionKeysNotInSet, print = false);

    Filter<uint8_t>* xorFilter8 = new XorFilter<uint8_t>();
    cout << "Running for Xor 8, # of keys: " << keyNum << ", fraction of keys not in set: " << fractionKeysNotInSet << endl;
    runExperiment(filenames, xorFilter8, keyNum, iterations, fractionKeysNotInSet, print = false);

    Filter<uint16_t>* xorFilter16 = new XorFilter<uint16_t>();
    cout << "Running for Xor 16, # of keys: " << keyNum << ", fraction of keys not in set: " << fractionKeysNotInSet << endl;
    runExperiment(filenames, xorFilter16, keyNum, iterations, fractionKeysNotInSet, print = false);

    delete bloom8;
    delete bloom10;
    delete bloom12;
    delete bloom16;
    delete xorFilter8;
    delete xorFilter16;
}

inline vector<string> formatFilenames(string folder){
    vector<string> newFilenames;
    for (string file : resultFilenames){
        newFilenames.push_back("../results/" + folder + "/" + file + ".csv");
    }
    return newFilenames;

}

inline void resetFiles(vector<string> filenames){
    for (string filename: filenames){
        ofstream outputFile(filename, ofstream::out | ofstream::trunc);
        outputFile.close();
    } 
}


inline void writeHeaders(vector<string> filenames, string independentVar){   
    writeToCSV(filenames, independentVar);
    writeToCSV(filenames, "Bloom8");
    writeToCSV(filenames, "Bloom10");
    writeToCSV(filenames, "Bloom12");
    writeToCSV(filenames, "Bloom16");

    writeToCSV(filenames, "Xor8");
    writeToCSV(filenames, "Xor16");
    writeToCSV(filenames, "Xor32");
    writeToCSV(filenames, "\n");
}


inline void runForKeys(size_t iterations, double fractionKeysNotInSet, string filename, bool print = false){
    vector<string> expFilenames = formatFilenames(filename);
    resetFiles(expFilenames);

    writeHeaders(expFilenames, "#Keys");

    vector<uint64_t> keyNums = {10000, 100000, 1000000, 10000000};
    for (const auto& keyNum : keyNums) {
        writeToCSV(expFilenames, to_string(keyNum));
        runForAll(expFilenames, keyNum, iterations, fractionKeysNotInSet);
        writeToCSV(expFilenames, "\n");
    }
    writeToCSV(expFilenames, "\n");
}

inline void runForFractionKeys(size_t iterations, size_t numKeys, bool print = false){
    vector<string> expFilenames = formatFilenames("fractionKeys");
    resetFiles(expFilenames);

    writeHeaders(expFilenames, "FractionKeysNotInSet");

    vector<double> fractions = {0.25, 0.5, 0.75};
    for (const auto& fraction : fractions) {
        writeToCSV(expFilenames, to_string(fraction));
        runForAll(expFilenames, numKeys, iterations, fraction);
        writeToCSV(expFilenames, "\n");
    }
    writeToCSV(expFilenames, "\n");
}

