#ifndef FILTER_H
#define FILTER_H

#include <iostream>
#include <vector>

using namespace std;
template<typename T>
class Filter {

public:
    virtual void build(vector<uint64_t>& keys) = 0;
    virtual bool contains(uint64_t key) = 0;
    virtual vector<T> getFilter() = 0;

    virtual ~Filter() {}
};

#endif