#include <iostream>
#include "BigInt.h"

using namespace std;

int main() {
    BigInt b1("99999999999999999999999999999999999999999"), b2("1000000000000000000000000");
    b1 = b1 - b2;
    b2--;
    cout << b2.getValue();
    return 0;
}