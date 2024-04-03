#include <random>
#include <iostream>
#include <cstdint>
#include <fstream>

using namespace std;

/*
Generate a random uint64 for seeding hashing purposes
*/
inline uint64_t generateRandomUInt64() {
    // Use a random device as a source of entropy
    random_device rd;

    // Use a random number engine and seed it with entropy from random device
    mt19937_64 gen(rd());

    // Define the range of the random numbers
    uniform_int_distribution<uint64_t> dis;

    // Generate a random uint64_t number
    return dis(gen);
}

/*
Calculate average of a vector
*/
inline float calculateAverage(const vector<float>& vec) {
    float sum = 0.0f;
    for (float value : vec) {
        sum += value;
    }
    return sum / vec.size();
}


/*
Save results to a csv file, write a single string to just one file 
*/
inline void writeToCSV(const string& filename, const string data) {
    ofstream file(filename, ios::app); // Open file in append mode
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename << endl;
        return;
    }

    file << data;
    if (data != "\n"){
        file << ","; 
    }
    file.close();
}

/*
Save results to a csv line, write a single string to several files. Used for titles that are the same across files
*/
inline void writeToCSV(const vector<string>& filenames, const string data) {
    for (string filename : filenames){
        ofstream file(filename, ios::app); // Open file in append mode
        if (!file.is_open()) {
            cerr << "Error: Unable to open file: " << filename << endl;
            return;
        }

        file << data;
        if (data != "\n"){
            file << ","; 
        }
        file.close();
    }
}