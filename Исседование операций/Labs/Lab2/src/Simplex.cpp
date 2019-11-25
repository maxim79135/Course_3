#include "Simplex.h"

Simplex::Simplex(MatrixXd source) {
    n = source.rows();
    m = source.cols();
    table = MatrixXd(n, n + m);

    for (int i = 0; i < n; i++) {
            for (int j = 0; j < table.cols(); j++) {
                if (j < n)
                    table(i, j) = source(i, j);
                else
                    table(i, j) = 0;
            }
        
            if ((m + i) < table.cols()) {
                table(i, m + i) = 1;
                basis.push_back(m + i);
            }
    }
 
    m = table.cols();
}

Simplex::~Simplex() {

}