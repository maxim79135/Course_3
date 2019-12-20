#include <iostream>

using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    int *countLamp = new int[n];
    for (int i = 0; i < n; i++) countLamp[i] = 0;
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        ++countLamp[a - 1];
        ++countLamp[b - 1];
    }

    for (int i = 0; i < n; i++) cout << countLamp[i] << " ";
    return 0;
}
