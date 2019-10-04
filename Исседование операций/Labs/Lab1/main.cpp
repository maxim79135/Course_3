#include <iostream>
#include <Eigen/Dense>

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::FullPivLU;

bool isFreeVariable(MatrixXd matrix, int number) {
	int k = number, tmp;
    while (!(tmp = matrix(k, k))) {
        k++;
        if (k == matrix.rows()) {
            int l = number;
            while (<#condition#>) {
                <#statements#>
            }
        }
    }	
}

void solveMatrixWithGauss(MatrixXd matrix, bool isOneSolve) {
	if (isOneSolve) 
		cout << "System has one solution" << endl;
	else
		cout << "System has more then one solution" << endl;
	
	double tmp;
	int n = matrix.rows(), m = matrix.cols() - 1;
	VectorXd result(n);
	for (int i = 0; i < n; i++) {
		/*int k = i;
		do {
			tmp = matrix(i, k);
			k++;
		} while (tmp == 0 && k <= m);
		k--;
		if (k == m) continue;*/
        
		for (int j = m; j >= k; j--) matrix(i, j) /= tmp;
		for (int j = i + 1; j < n; j++) {
			tmp = matrix(j, k);
			for (int l = m; l >= k; l--) matrix(j, l) -= tmp * matrix(i, l);
		}
		cout << matrix << "\n\n";
	}
	cout << "Result\n";
	
	MatrixXd res = MatrixXd::Zero(m + 1, m + 1);
	res(m, m) = 1;
	for (int i = n; i < m; i++) res(i, i) = 1;
		
	for (int i = n - 1; i >= 0; i--) {
		for (int j = 0; j < m + 1; j++) res.row(i) -= matrix(i, j) * res.row(j) * (j == m ? -1 : 1);
	}

	for (int i = 0; i < m; i++) {
		cout << "X" << i + 1 << " = ";
		bool isZeroCountX = false;
		if (res(i, m)) {
			cout << res(i, m);
			isZeroCountX = true;
		}
		for (int j = 0; j < m; j++) {
			if (res(i, j) != 0) {
				isZeroCountX = true;	
				if (abs(res(i, j)) == 1) 
					cout << ((res(i, j) > 0) ? "+" : "-") << "x" << j + 1;
				else {
					if (res(i, j) > 0) cout << "+";
					cout << res(i, j) << "*x" << j + 1;
				}
			}
		}
		if (!isZeroCountX) cout << 0;
		cout << endl;
	}
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
	} else
		solveMatrixWithGauss(Ab, rankA == m ? true : false);	

}
