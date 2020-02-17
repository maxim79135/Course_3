#include <iostream>

using namespace std;

int main() {
	int n, m;
	cin >> n >> m;
	int tmp1, tmp2;

	int** a = new int*[n];
	for (int i = 0; i < n; i++)
			a[i] = new int[n];

	int* res = new int[n];	
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			a[i][j] = 0;

	for (int i = 0; i < n; i++) res[i] = 0;
	for (int i = 0; i < m; i++) {
		cin >> tmp1 >> tmp2;
		a[tmp1 - 1][tmp2 - 1]++;
	}

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i != j) {
				if (a[i][j] + a[i][j] != 1) {
					cout << "NO";
					return 0;
				}
			}
	cout << "YES";	
}

