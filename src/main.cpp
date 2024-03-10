#include <iostream>
#include "XorFilter.h"
#include <vector>

using namespace std;

int main() {
    cout << "XOR FILTER! " << endl;
    vector<uint64_t> keys = {2, 4, 5, 7};
    XorFilter* xorFilter = new XorFilter(keys);
    cout << xorFilter->contains(2) << endl;
    delete xorFilter;
}
