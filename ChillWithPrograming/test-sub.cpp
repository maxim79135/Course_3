#include <iostream>
#include "BigInt.h"

using namespace std;

int main() {
    BigInt b1, b2;
    cin >> b1 >> b2;
    b1 = b1 - b2;
    b2--;
    cout << b2 << " " << b1;
    return 0;
}