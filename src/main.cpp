#include <iostream>
#include "XorFilter.h"
#include <vector>

using namespace std;

int main() {
    cout << "Hello, World! 2" << endl;
    vector<uint64_t> keys = {2, 4, 5, 7};
    cout << "fuck";
    XorFilter* xorFilter = new XorFilter(keys);

    delete xorFilter;
}
