#include "XorFilter.h"
#include "xxhash.h"
#include <string>
#include <utility>  // Include this header for std::pair
#include <cstring>  // Include this header for memcpy
#include <fstream>


using namespace std;
XorFilter::XorFilter(double numBitsPerEntry, int64_t expectedNumElements) {
    int numHashFunctions = static_cast<int>(std::ceil(std::log(2) * numBitsPerEntry));
    int64_t arraySize = numBitsPerEntry * expectedNumElements;
    size_t bitArraySize = (arraySize + 7) / 8;  // Adjusted for uint8_t
    this->filter = new std::vector<uint8_t>;
    this->filter->resize(bitArraySize);

    for (int i = 0; i < numHashFunctions; ++i) {
        hashFunctions.emplace_back([i](const int64_t& key) {
            return XXH64(&key, sizeof(key), i);
        });
    }
}

void XorFilter::insert(const int64_t& key) {
    for (const auto& hashFunction : hashFunctions) {
        size_t index = hashFunction(key) % (filter->size() * 8);
        filter->at(index / 8) |= (1 << (index % 8));
    }
}

bool XorFilter::contains(const int64_t& key) {
    for (const auto& hashFunction : hashFunctions) {
        size_t index = hashFunction(key) % (filter->size() * 8);
        if ((filter->at(index / 8) & (1 << (index % 8))) == 0) {
            return false;
        }
    }
    return true;
}

XorFilter::XorFilter(std::vector<uint8_t>* filter, double numBitsPerEntry) {
    int numHashFunctions = static_cast<int>(std::ceil(std::log(2) * numBitsPerEntry));

    this->filter = filter;
    for (int i = 0; i < numHashFunctions; ++i) {
        hashFunctions.emplace_back([i](const int64_t& key) {
            return XXH64(&key, sizeof(key), i);
        });
    }
}


void XorFilter::write(string fileName) {
    size_t totalBits = this->filter->size() * 8;  // Convert from bytes to bits
    size_t bitsPerPage = 4096 * 8;
    size_t fullPages = totalBits / bitsPerPage;
    size_t remainingBits = totalBits % bitsPerPage;
    vector<pair<char*, int>> pagesBloomFilter;

    for (size_t i = 0; i < fullPages; ++i) {
        size_t startIndex = i * bitsPerPage / 8;
        char* pageBloom = new char[4096];

        std::memcpy(pageBloom, this->filter->data() + startIndex, bitsPerPage / 8);

        pagesBloomFilter.push_back(make_pair(pageBloom, 4096));
    }

    if (remainingBits > 0) {
        size_t startIndex = fullPages * bitsPerPage / 8;
        size_t remainingBytes = (remainingBits + 7) / 8;
        char* pageBloom = new char[4096];

        std::memcpy(pageBloom, this->filter->data() + startIndex, remainingBytes);

        // Pad the remaining space with zeros
        memset(pageBloom + remainingBytes, 0, 4096 - remainingBytes);
        pagesBloomFilter.push_back(make_pair(pageBloom, 4096));
    }

    ofstream outfile2(fileName, ios::app | ios::binary);

    for (auto& page : pagesBloomFilter) {
        outfile2.write(page.first, page.second);
        delete[] page.first;
    }

    outfile2.close();
}