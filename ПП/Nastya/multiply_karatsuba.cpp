#include <vector>
#include "multiply_karatsuba.h"

using namespace std;

vector<int> multiply_karatsuba(const vector<int>& x, const vector<int>& y) {
    auto len = x.size();
    vector<int> res(2 * len);

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