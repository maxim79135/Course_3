#include <iostream>
#include <Eigen/Dense>

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::FullPivLU;

VectorXd solveMatrixWithGauss(MatrixXd matrix) {
	double tmp;
	int n = matrix.rows(), m = matrix.cols() - 1;
	VectorXd result(n);
	for (int i = 0; i < n; i++) {
		int k = i;
		do {
			tmp = matrix(i, k);
			k++;
		} while (tmp == 0 || k <= m);
		if (k == m) 
		k--;
		for (int j = m; j >= k; j--) matrix(i, j) /= tmp;
		for (int j = i + 1; j < n; j++) {
			tmp = matrix(j, k);
			for (int l = m; l >= k; l--) matrix(j, l) -= tmp * matrix(i, l);
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
	cout << "Type of system: Ax=b\n";
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
	FullPivLU<MatrixXd> lu(A);
	FullPivLU<MatrixXd> lu2(Ab);
	int rankA = lu.rank();
	int rankAb = lu2.rank();
	
	if (rankA != rankAb) {
		cout << "System has not solutions" << endl;
		return EXIT_SUCCESS;
	} else if (rankA == m) {
		cout << "System has one solution" << endl;
		VectorXd v = solveMatrixWithGauss(Ab);
		cout << "Result\n";
		for (int i = 0; i < n; i++) cout << "X" << i + 1 << " = " << v(i) << endl; 
	} else {
		cout << "System has more then one solution" << endl;
		VectorXd v = solveMatrixWithGauss(Ab);	
	}
}