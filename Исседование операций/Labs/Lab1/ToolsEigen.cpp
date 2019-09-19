#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;

int main() {
    int n, m;
    std::cin >> n >> m;

    MatrixXd matrix(n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            std::cin >> matrix(i, j);
    
    std::cout << matrix << std::endl;
}