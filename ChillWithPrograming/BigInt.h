#pragma once
#include <string>

using std::string;

class BigInt{
    public:
        BigInt() { value = ""; }
        BigInt(string v) { value = v; }
        
        BigInt operator- (BigInt bi);
        BigInt& operator-- ();
        BigInt operator-- (int);
        friend std::ostream& operator<< (std::ostream &out, const BigInt &bi);
    
        string getValue() { return this->value; }
    private:
        string value;
};