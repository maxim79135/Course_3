#include <iostream>

using namespace std;

int main() {
	int n;
	cin >> n;

	int** a = new int*[n];
	for (int i = 0; i < n; i++)
			a[i] = new int[n];

	for (int i = 0; i < n; i++)			
		for (int j = 0; j < n; j++)
			cin >> a[i][j];

	int* in = new int[n];
	int* out = new int[n];
	for (int i = 0 ; i < n; i++) {
		in[i] = 0;
		out[i] = 0;
	}

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			if (a[i][j] == 1) {
				in[j]++;
				out[i]++;
			}
		}

	int count_in = 0;
	int count_out = 0;

	for (int i = 0; i < n; i++) {
		if (in[i] == 0) count_in++;
		if (out[i] == 0) count_out++;
	}
	cout << count_in << endl;
	for (int i = 0; i < n; i++) if (in[i] == 0) cout << i + 1 << endl;
	cout << count_out << endl;
	for (int i = 0; i < n; i++) if (out[i] == 0) cout << i + 1 << endl;
}
