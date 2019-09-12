#include <iostream>
#include <string>
#include <fstream>

using namespace std;

bool isLess(string s1, string s2) {
    if (s1.size() < s2.size()) return true ;
    else
    if (s1.size() > s2.size()) return false;
    else {
        int i = 0;
        if (s1 == s2) return false;
        while (i < s1.size()) {
            if (s1[i] > s2[i]) return false;
            if (s1[i] < s2[i]) return true;
            i++;
        }
    }
}

string sub(string s1, string s2) {
    for (int i = s2.size() - 1, j = s1.size() - 1; i >= 0; i--, j--) {
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
    return s1.empty() ? "0" : s1;
}

int main() {
    ifstream in;
    ofstream out;
    in.open("input.txt");
    out.open("output.txt");
    string s1, s2;
    in >> s1 >> s2;

    int div = 0;
    while (!isLess(s1, s2)) {
        s1 = sub(s1, s2);
        div++;
    }
    
    out << div << endl << s1;
    out.close();
    in.close();
    return 0;
}