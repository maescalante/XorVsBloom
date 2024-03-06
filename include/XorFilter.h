#ifndef XOR_FILTER_H
#define XOR_FILTER_H

#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <bitset>
#include <string>

class XorFilter {
private:

    std::vector<std::function<size_t(const int64_t&)>> hashFunctions;

public:
    std::vector<uint8_t> *filter;
    XorFilter(double numBitsPerEntry, int64_t expectedNumElements);
    XorFilter(std::vector<uint8_t>* filter, double numBitsPerEntry);

    void insert(const int64_t& key);
    bool contains(const int64_t& key);
    void write(std::string filename);
};

#endif