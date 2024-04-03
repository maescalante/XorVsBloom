#include <iostream>
#include <vector>
#include "experiments.cpp"
#include "Filter.h"

using namespace std;

int main() {
    cout << "XOR FILTER! " << endl;
    size_t iterations = 5;

    runForKeys(iterations, 0.2, "keys20");
    runForKeys(iterations, 0.8, "keys80");
    runForFractionKeys(iterations, 1000000);

}
