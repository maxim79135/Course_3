#include "BigInt.h"

BigInt BigInt::operator- (BigInt bi) {
    string s1 = this->value, s2 = bi.value;
    for (int i = bi.value.size() - 1, j = s1.size() - 1; i >= 0; i--, j--) {
        if (s1[j] >= s2[i]) {
            s1[j] = char(int(s1[j]) - int(s2[i]) + 48);
        } else {
            s1[j] = char(int(s1[j]) - int(s2[i]) + 58);
            int k = j - 1;
            while (s1[k] == '0') {
                s1[k] = '9';
                k--;
            }
                s1[k] = char(int(s1[k]) - 1);
        }
    }
    while (s1[0] == '0') s1.erase(0, 1);    
    return BigInt(s1);
}

BigInt& BigInt::operator--() {
    *this = *this - BigInt("1");
    return *this;
}

BigInt BigInt::operator--(int) {
    BigInt temp = *this;
    --*this;
    return temp;
}

std::ostream& operator<< (std::ostream &out, const BigInt &bi) {
    out << bi.value;
    return out;
}