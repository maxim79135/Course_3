#include <iostream>

using namespace std;

int main() {
    int n;
    cin >> n;
    int **matrix = new int*[n];
    for (int i = 0; i < n; i++) matrix[i] = new int[n];

    bool flag = false;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            cin >> matrix[i][j];
            if (i == j && matrix[i][j]) flag = true;
        }

    flag ? cout << "YES" : cout << "NO";
    return EXIT_SUCCESS;
}