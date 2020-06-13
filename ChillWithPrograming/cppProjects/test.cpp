#include <complex>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

int main() {
    vector<complex<double>> arr;
    arr.push_back(1i * 1i);
    arr.push_back(3i * 1i);
    arr.push_back(2i * 1i);

    double *a = new double[arr.size() * 2];
    for (int i = 0; i < arr.size(); i++) {
        a[i*2] = arr[i].real();
        a[i*2+1] = arr[i].imag();
    }
    for (int i = 0; i < arr.size() * 2; i++) {
        cout << a[i] << " ";
    }
}