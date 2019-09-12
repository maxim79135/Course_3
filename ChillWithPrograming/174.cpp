#include <iostream>

using namespace std;

int main() {
    int n;
    cin >> n;
    int **ways = new int*[n];
    for (int i = 0; i < n; i++) {
        ways[i] = new int[n];
    }
    int k = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            cin >> ways[i][j];
            if (ways[i][j]) k++;
        }
    cout << k / 2;
    return 0;
}