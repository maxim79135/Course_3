#include "Simplex.h"

int main() {
    MatrixXd table(3, 3);
    table << 1, 2, 3,
             4, 5, 6,
             7, 8, 9;

    Simplex *s = new Simplex(table);
}
