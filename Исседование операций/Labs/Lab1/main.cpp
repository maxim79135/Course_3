#include <iostream>
#include <iomanip>
#include <Eigen/Dense>

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;

/*double matrix [4][5] = {{0.73, 1.24, -0.38,-1.43, 0.58}, 
	                    {1.07, -0.77, 1.25, 0.66, 0.66}, 
						{1.56, 0.66, 1.44, -0.87, 1.24}, 
						{0.75, 1.22, -0.83, 0.87, 0.92} };

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
*/

VectorXd standartGauss(MatrixXd matrix) {
	double tmp;
	int n = matrix.rows(), m = matrix.cols();
	VectorXd result(n);
	for (int i = 0; i < n; i++) {
		tmp = matrix(i, i);
		for (int j = n; j >= i; j--) matrix(i, j) /= tmp;
		for (int j = i + 1; j < n; j++) {
			tmp = matrix(j, i);
			for (int k = n; k >= i; k--) matrix(j, k) -= tmp * matrix(i, k);
		}
		cout << matrix << "\n\n";
	}

	result(n - 1) = matrix(n - 1, n);
	for (int i = n - 2; i >= 0; i--) {
		result(i) = matrix(i, n);
		for (int j = i + 1; j < n; j++) result(i) -= matrix(i, j) * result(j);
	}
	return result;
}

int main() {
	cout << "Solving a fourth-order linear equation system by the Gauss method" << endl;
	cout << "Type of system: Ax=b";
	int n, m;
	cout << "Enter n and m for matrix: ";
	cin >> n >> m;

	MatrixXd A(n, m);
	VectorXd b(n);

	//read matrix
	cout << "Enter matrix A" << endl;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			cin >> A(i, j);

	// column free coefficient
	cout << "Enter free coefficient" << endl;
	for (int i = 0; i < n; i++) cin >> b(i);
	MatrixXd Ab = A;
	Ab.conservativeResize(Ab.rows(), Ab.cols() + 1);
	Ab.col(Ab.cols() - 1) = b;

	// find ranks
	Eigen::FullPivLU<MatrixXd> lu(A);
	Eigen::FullPivLU<MatrixXd> lu2(Ab);
	int rankA = lu.rank();
	int rankAb = lu2.rank();
	
	if (rankA != rankAb) {
		cout << "System has not solutions" << endl;
		return EXIT_SUCCESS;
	} else if (rankA == n) {
		VectorXd v = standartGauss(Ab);
		cout << "Result\n";
		for (int i = 0; i < n; i++) cout << "X" << i + 1 << " = " << v(i) << endl; 
	}
}