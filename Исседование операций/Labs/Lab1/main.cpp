#include <iostream>
#include <Eigen/Dense>

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::FullPivLU;

bool isFreeVariable(MatrixXd matrix, int number) {
	int k = number, tmp;
    while (!(tmp = matrix(k, number))) {
        k++;
        if (k == matrix.rows()) {
            return true;
        }
    }
    return false;
}

void swapRowsInMatrix(MatrixXd &matrix, int i, int j) {
    VectorXd temp(matrix.cols());
    temp = matrix.row(i);
    matrix.row(i) = matrix.row(j);
    matrix.row(j) = temp;
}

void solveMatrixWithGauss(MatrixXd matrix, bool isOneSolve) {
    // free var on matrix
    VectorXd freeVariables = VectorXd::Zero(matrix.cols() - 1);
    if (isOneSolve)
		cout << "System has one solution" << endl;
	else
		cout << "System has more then one solution" << endl;
	
	double tmp;
	int n = matrix.rows(), m = matrix.cols() - 1, i = 0;
	while (i < n) {
        int k = i;
        if (isFreeVariable(matrix, i)) {
            if (matrix.row(i).isZero()) {
                i++;
                continue;
            } else {
                int l = i;
                while (!(tmp = matrix(i, l))) {
                    l++;
                    k = l;
                }
            }
            freeVariables(i) = 1;
        } else {
            while (!(tmp = matrix(k, i))) {
                k++;
            }
            swapRowsInMatrix(matrix, i, k);
            k = i;
        }
		for (int j = m; j >= k; j--) matrix(i, j) /= tmp;
		for (int j = i + 1; j < n; j++) {
			tmp = matrix(j, k);
			for (int l = m; l >= k; l--) matrix(j, l) -= tmp * matrix(i, l);
		}
		cout << matrix << "\n\n";
        //i = k;
        i++;
	}
	cout << "Result\n";

    matrix.conservativeResize(matrix.rows() + 2, matrix.cols());
    for (int i = 0; i < m - n; i++) matrix.row(i + n) = VectorXd::Zero(m + 1);

    for (int i = n - 1; i > 0; i--)
        for (int j = 0; j < m + 1; j++) {
            if (matrix(i, j)) {
                swapRowsInMatrix(matrix, i, j);
                break;
            }
        }

    //cout << endl << matrix << endl << endl;
    MatrixXd res = MatrixXd::Zero(m + 1, m + 1);
    res(m, m) = 1;
    for (int i = 0; i < m; i++) if (!matrix(i, i)) res(i, i) = 1;
    
    
    //cout << endl << matrix << endl;
	for (int i = m - 1; i >= 0; i--) {
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
	system("clear");
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
	long rankA = lu.rank();
	long rankAb = lu2.rank();
	
	if (rankA != rankAb) {
		cout << "System has not solutions" << endl;
		return EXIT_SUCCESS;
	} else
		solveMatrixWithGauss(Ab, rankA == m ? true : false);	

}
