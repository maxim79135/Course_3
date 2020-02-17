#include <iostream>

using namespace std;

int main() {
	int n, m;
	cin >> n >> m;
	int tmp;
	int* res = new int[n];	
	for (int i = 0; i < n; i++) res[i] = 0;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < 2; j++) {
			cin >> tmp;
			res[tmp - 1]++;
		}
	for (int i = 1; i < n; i++) if (res[i] != res[0]) {
		cout << "NO";
		return 0;
	}
	cout << "YES";
}

