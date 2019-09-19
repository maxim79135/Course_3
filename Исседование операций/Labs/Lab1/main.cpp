#include <iostream>
#include <iomanip>
#include <Eigen/Dense>

using namespace std;

/*double matrix [4][5] = {{0.73, 1.24, -0.38,-1.43, 0.58}, 
	                    {1.07, -0.77, 1.25, 0.66, 0.66}, 
						{1.56, 0.66, 1.44, -0.87, 1.24}, 
						{0.75, 1.22, -0.83, 0.87, 0.92} };
*/

void show_matrix(double** matrix, int n, int m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) cout << fixed << setprecision(5) << matrix[i][j] << "*x" << j + 1 << " " << setw(5);
		cout << endl;
	}
	cout << endl;
}

int findRangMatrix(double** matrix, int n, int m) {
	
}

int main() {
	cout << "Solving a fourth-order linear equation system by the Gauss method" << endl;
	int n, m;
	cout << "Enter n and m for matrix: ";
	cin >> n >> m;
	double **matrix = new double*[n];
	for (int i = 0; i < n; i++) matrix[i] = new double[m];

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			cin >> matrix[i][j];

	show_matrix(matrix, n, m);

	double tmp, *xx = new double[m];
	for (int i = 0; i < n; i++) {
		tmp = matrix[i][i];
		for (int j = n; j >= i; j--) matrix[i][j] /= tmp;
		for (int j = i + 1; j < n; j++) {
			tmp = matrix[j][i];
			for (int k = n; k >= i; k--) matrix[j][k] -= tmp * matrix[i][k];
		}
		show_matrix(matrix, n, m);
	}

	xx[n - 1] = matrix[n - 1][n];
	for (int i = n - 2; i >= 0; i--) {
		xx[i] = matrix[i][n];
		for (int j = i + 1; j < n; j++) xx[i] -= matrix[i][j] * xx[j];
	}

	for (int i = 0; i < n; i++) cout << "x" << i + 1 << " = " << xx[i] << endl;
	
	//clear memory
	for (int i = 0; i < n; i++) delete[] matrix[i];
	delete[] matrix;
	delete[] xx;
}