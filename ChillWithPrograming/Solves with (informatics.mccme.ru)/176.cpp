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
    
    getchar();
    int *colors = new int[n];

    int count = 0;
    for (int i = 0; i < n; i++) {
        cin >> colors[i];
    }

    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++) {
            if (matrix[i][j] && colors[i] != colors[j]) count++;
        }
    
    cout << count;
    return EXIT_SUCCESS;
}