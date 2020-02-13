#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

vector<int> naive_mul(const vector<int>& x, const vector <int>& y) {
    auto len = x.size();
    vector<int> res(2 * len);

    for (auto i = 0; i < len; i++) {
        for (auto j = 0; j < len; j++) {
            res[i + j] += x[i] * y[j];
        }
    }
    return res;
}

vector<int> multiply_karatsuba(const vector<int>& x, const vector<int>& y) {
    auto len = x.size();
    vector<int> res(2 * len);

    if (len <= 128) {
        return naive_mul(x, y);
    }

    auto k = len / 2;

    vector<int> Xr {x.begin(), x.begin() + k};
    vector<int> Xl {x.begin() + k, x.end()};
    vector<int> Yr {y.begin(), y.begin() + k};
    vector<int> Yl {y.begin() + k, y.end()};

    vector<int> P1 = multiply_karatsuba(Xl, Yl);
    vector<int> P2 = multiply_karatsuba(Xr, Yr);

    vector<int> Xlr(k);
    vector<int> Ylr(k);

    for (int i = 0; i < k; i++) {
        Xlr[i] = Xl[i] + Xr[i];
        Ylr[i] = Yl[i] + Yr[i];
    }

    vector<int> P3 = multiply_karatsuba(Xlr, Ylr);

    for (auto i = 0; i < len; i++) {
        P3[i] -= P2[i] + P1[i];
    }

    for (auto i = 0; i < len; i++) {
        res[i] = P2[i];
    }

    for (auto i = len; i < 2 * len; i++) {
        res[i] = P1[i - len];
    }

    for (auto i = k; i < len + k; i++) {
        res[i] += P3[i - k];
    }
    return res;
}

vector<int> get_number(ifstream& is) {
    string snum;
    vector<int> vnum;
    unsigned int dig = 1;
    int n = 0;

    is >> snum;
    
    for (auto it = snum.crbegin(); it != snum.crend(); it++) {
        n += (*it - '0') * dig;
        dig *= 10;

        if (dig == 1e7) {
            vnum.push_back(n);
            n = 0;
            dig = 1;
        }
    }

    if (n != 0) {
        vnum.push_back(n);
    }

    return vnum;
}

int main() {
    ifstream fin;
    ofstream fout;
    fin.open("input.txt");
    fout.open("output.txt");

    vector<int> a = get_number(fin);
    vector<int> b = get_number(fin);

    //vector<int> a = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    //vector<int> b = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<int> res = multiply_karatsuba(a, b);
    for (auto i = 0; i < res.size(); i++) {
        fout << res[i];
    }
    return 0;
}