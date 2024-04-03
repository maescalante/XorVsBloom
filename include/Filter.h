#ifndef FILTER_H
#define FILTER_H

#include <iostream>
#include <vector>

using namespace std;
// Abstract class for methods that a filter should contain
template<typename T>
class Filter {

public:
    virtual void build(vector<uint64_t>& keys) = 0;
    virtual bool contains(uint64_t key) = 0;
    virtual vector<T> getFilter() = 0;
    virtual int getMemoryOccupied() = 0 ;
    virtual void resetFilter() = 0;

    virtual ~Filter() {}
};

#endif