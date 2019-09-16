#include <iostream>

using namespace std;

int main() {
    int n;
    cin >> n;
    int **matrix = new int*[n];
    for (int i = 0; i < n; i++) matrix[i] = new int[n];
    
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> matrix[i][j];

    int minDist = 10000000;
    int minA, minB, minC;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                if (matrix[i][j] + matrix[j][k] + matrix[k][i] < minDist && matrix[i][j] != 0 && matrix[j][k] != 0 && matrix[k][i] != 0) {
                    minDist = matrix[i][j] + matrix[j][k] + matrix[k][i];
                    minA = i + 1; minB = j + 1; minC = k + 1;
                }

    cout << minA << " " << minB << " " << minC;
    return EXIT_SUCCESS;
}