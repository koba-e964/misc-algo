#include <vector>
#include <iostream>

#include "Eigen/Core"
using namespace std;
using namespace Eigen;


/*
 * From CiCANT
 */

template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
void red(int k,
	 int l,
	 Matrix<Scalar, Cols, Cols, Options, MaxCols, MaxCols> &mu,
	 Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &basis,
	 Matrix<Scalar, Cols, Cols, Options, MaxCols, MaxCols> &h) {
  if (abs(mu(k, l)) <= 0.5) {
    return;
  }
  Scalar q = floor(mu(k, l) + 0.5);
  basis.col(k) -= q * basis.col(l);
  mu(k, l) -= q;
  h.col(k) -= q * h.col(l);
  for (int i = 0; i < l; ++i) {
    mu(k, i) -= q * mu(l, i);
  }
}

template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
void swap(int k, Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &basis,
	  Matrix<Scalar, Cols, Cols, Options, MaxCols, MaxCols> &mu,
	  vector<Scalar> &cb,
	  int kmax,
	  Matrix<Scalar, Cols, Cols, Options, MaxCols, MaxCols> &h) {
  for (int i = 0; i < mu.cols(); ++i) {
    swap(basis(i, k), basis(i, k - 1));
    swap(h(i, k), h(i, k - 1));
  }
  for (int j = 0; j < k - 1; ++j) {
    swap(mu(k, j), mu(k - 1, j));
  }
  Scalar m = mu(k, k - 1), b = cb[k] + m * m * cb[k - 1];
  mu(k, k - 1) = m * cb[k - 1] / b;
  cb[k] *= cb[k - 1] / b;
  cb[k - 1] = b;
  for (int i = k + 1; i <= kmax; ++i) {
    Scalar t = mu(i, k);
    mu(i, k) = mu(i, k - 1) - m * t;
    mu(i, k - 1) = t + mu(k, k - 1) * mu(i, k);
  }
}


template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
Matrix<Scalar, Cols, Cols, Options, MaxCols, MaxCols>
lll_reduce(Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &basis) {
  int k = 1, kmax = 0;
  int n = basis.cols();
  int m = basis.rows(); // dim b[i]
  Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> bc(m, n); // b*
  bc.col(0) = basis.col(0);
  vector<Scalar> cb(n); // B
  cb[0] = bc.col(0).squaredNorm();
  typedef Matrix<Scalar, Cols, Cols, Options, MaxCols, MaxCols> MS;
  MS mu(n, n);
  MS h(MatrixXd::Identity(n, n));
  // Step 2
  do {
    if (k > kmax) {
      kmax = k;
      bc.col(k) = basis.col(k);
      for (int j = 0; j < k; ++j) {
	mu(k, j) = basis.col(k).dot(bc.col(j)) / cb[j];
	bc.col(k) -= mu(k, j) * bc.col(j);
      }
      cb[k] = bc.col(k).squaredNorm();
      if (cb[k] == 0) {
	cerr << "basis did not form a basis" << endl;
	abort();
      }
    }
    //Step 3
    while (1) {
      red(k, k - 1, mu, basis, h);
      if (cb[k] < (0.75 - pow(mu(k, k - 1), 2.0)) * cb[k - 1]) {
	swap(k, basis, mu, cb, kmax, h);
	k = max(1, k - 1);
	continue;
      }
      break;
    }
    for (int l = k - 2; l >= 0; --l) {
      red(k, l, mu, basis, h);
    }
    ++k;
  } while (k < n);
  return h;
}

int main(void) {
  MatrixXd basis(3, 3);
  basis << 1, -1, 3,
  1, 0, 5,
  1, 2, 6;
  MatrixXd h = lll_reduce(basis);
  for (int i = 0; i < 3; ++i) {
    cout << "basis[" << i << "]=" << basis.col(i) << endl;
  }
  cout << "H = " << h << endl;
}
