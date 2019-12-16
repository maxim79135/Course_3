#include <iostream>
#include <vector>
#include <limits>
#include <limits.h>

using namespace std;

int main() {
	int count, MIN = INT_MAX;
	cin >> count;
	vector <vector <int> > matrix(count, vector<int>(count));
	vector <vector <int> > weights = matrix;
	for (int i = 0; i < count; i++)
		for (int j = 0; j < count; j++) {
			cin >> matrix[i][j];
			if (matrix[i][j] == 0) { weights[i][j] = INT_MAX; }
			else { weights[i][j] = matrix[i][j]; }
		}

	for (int k = 0; k < count; k++)
		for (int i = 0; i < count; i++)
			for (int j = 0; j < count; j++)
				if (weights[i][j] != INT_MAX)
					if (weights[i][k] == INT_MAX || weights[k][j] == INT_MAX)
						weights[i][j] = min(weights[i][j], INT_MAX);
					else
						weights[i][j] = min(weights[i][j], weights[i][k] + weights[k][j]);
				else 
					if (weights[i][k] == INT_MAX || weights[k][j] == INT_MAX)
						weights[i][j] = INT_MAX;
					else				
						weights[i][j] = weights[i][k] + weights[k][j];
	
	for (int i = 0; i < count; i++)
		for (int j = 0; j < count; j++)
			if (weights[i][j] < 0) {
				cout << -1;
				return 0;
			}
			else {
				if (weights[i][j] < MIN && i != j) MIN = weights[i][j];
			}
	if (MIN == INT_MAX) cout << -1;
	else cout << MIN;
}
