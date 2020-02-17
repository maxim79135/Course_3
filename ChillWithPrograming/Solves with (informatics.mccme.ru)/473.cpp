#include <iostream>

using namespace std;

int main() {
	int n, m;
	cin >> n >> m;
	int tmp1, tmp2;
	int* in = new int[n];	
	int* out = new int[n];
	for (int i = 0; i < n; i++) {
			in[i] = 0;
			out[i] = 0;
	}
	for (int i = 0; i < m; i++) {
			cin >> tmp1 >> tmp2;
			out[tmp1 - 1]++;
			in[tmp2 - 1]++;
	}
	for (int i = 0; i < n; i++) {
		cout << in[i] << endl;
		cout << out[i] << endl;
	}
}
