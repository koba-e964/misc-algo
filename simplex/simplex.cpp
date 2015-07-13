#include <vector>
#include <iostream>

#include "Eigen/Core"

using namespace std;
using namespace Eigen;
/*
 * Solves maxarg(c x) s.t. Ax = b, x >= 0
 */
VectorXd simplex(const MatrixXd &A, const VectorXd &b, const VectorXd &c) {
  assert (A.rows() == b.rows());
  assert (A.cols() == c.rows());
  assert (A.rows() <= A.cols());
  int m = A.rows(), n = A.cols();
  vector<int> basis;
  for (int i = n - m; i < n; ++i) {
    basis.push_back(i);
  }
  MatrixXd Ac = A;
  VectorXd bc = b, cc = c;
  while (1) {
    bool is_opt = true;
    int pivot = -1;
    for (int i = 0; i < n; ++i) {
      is_opt &= cc[i] <= 0;
      if (cc[i] > 0) {
	pivot = i;
      }
    }
    if (is_opt) {
      break;
    }
    int mini = -1;
    double lim = 1.0 / 0.0;
    for (int i = 0; i < m; ++i) {
      if (Ac(i, pivot) == 0) {
	continue;
      }
      double ilim = bc[i] / Ac(i, pivot);
      if (lim > ilim) {
	mini = i;
	lim = ilim;
      }
    }
    assert (mini >= 0);
    bc[mini] /= Ac(mini, pivot);
    Ac.row(mini) /= Ac(mini, pivot);
    for (int i = 0; i < m; ++i) {
      if (i != mini) {
	double r = Ac(i, pivot);
	Ac.row(i) -= r * Ac.row(mini);
	bc[i] -= r * bc[mini];
      }
    }
    cc -= Ac.row(mini) * cc[pivot];
    basis[mini] = pivot;
  }
  VectorXd res = VectorXd::Zero(n);
  for (int i = 0; i < m; ++i) {
    res[basis[i]] = bc[i];
  }
  return res;
}

/*
 * Solves maxarg(c x) s.t. Ax <= b, x >= 0
 */
VectorXd simplex_le(const MatrixXd &A, const VectorXd &b, const VectorXd &c) {
  MatrixXd Ac(A);
  VectorXd cc(c);
  int m = A.rows();
  int n = A.cols();
  Ac.conservativeResize(m, n + m);
  cc.conservativeResize(n + m);
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < m; ++j) {
      Ac(i, j + n) = i == j ? 1 : 0;
    }
    cc(i + n) = 0;
  }
  VectorXd r = simplex(Ac, b, cc);
  r.conservativeResize(n);
  return r;
}

int main(void) {
  MatrixXd A(2,2);
  A <<
    1,2,
    2,1;
  VectorXd b(2), c(2);
  b << 2, 2;
  c << 1, 1;
  VectorXd res = simplex_le(A, b, c);
  cout << "result = " << res << endl;
}
