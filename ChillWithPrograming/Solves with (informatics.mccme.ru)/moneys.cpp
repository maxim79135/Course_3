#include <iostream>

using namespace std;

int main() {
    int w, n = 4;
    cin >> w;

    int P[4];
    P[0] = 1; P[1] = 2; P[2] = 5; P[3] = 10;
    int matrix[n][w + 1];
    /*
    int **matrix = new int*[n];
    for (int i = 0; i < n; i++)
        matrix[i] = new int[w + 1];
    */
    for (int i = 0; i < n; i++)
        matrix[i][0] = 1;

    for (int j = 0; j < w + 1; j++)
        matrix[0][j] = 1;

    for (int i = 1; i < n; i++)
        for (int j = 1; j < w + 1; j++)
            if (j < P[i])
                matrix[i][j] = matrix[i - 1][j];
            else
                matrix[i][j] = matrix[i - 1][j] + matrix[i][j - P[i]];
    cout << matrix[n - 1][w];
}