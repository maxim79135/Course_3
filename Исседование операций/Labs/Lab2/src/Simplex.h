#pragma once
#include <vector>
#include <eigen3/Eigen/Dense>

using std::vector;
using Eigen::MatrixXd;

class Simplex
{
private:
    int n, m;
    MatrixXd table;

    vector<int> basis;
public:
    Simplex(MatrixXd source);
    ~Simplex();
};
