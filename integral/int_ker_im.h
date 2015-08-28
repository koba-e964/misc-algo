#ifndef INT_KER_IM_H
#define INT_KER_IM_H
#include "lll.h"
#include "Eigen/LU"

template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
Matrix<Scalar, Cols, Cols, Options, MaxCols, MaxCols>
  ker_im(const Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &A, int &p, Scalar eps) {
  int n = A.cols();
  Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> Acp(A);
  // Step 1, Apply MLLL
  cout << "step1" << endl;
  Matrix<Scalar, Cols, Cols, Options, MaxCols, MaxCols> H = dependent_lll_reduce(Acp, p, eps);
  int r = n - p;
  // Step 2, LLL-reduce the kernel
  cout << "step2"<<endl;
  Matrix<Scalar, Cols, Dynamic, Options, MaxCols, Dynamic> H2 = H;
  H2.resize(n, r);
  Matrix<Scalar, Cols, Dynamic, Options, Dynamic, Dynamic> H3 = integral_lll_reduce(H2);
  cout << "int_lll done" << endl;
  H = H3 * H;
  // Step 3, Compute inverse of Gram matrix
  cout << "step3" << endl;
  Matrix<Scalar, Dynamic, Dynamic, Options, Dynamic, Dynamic> C(r, r);
  for (int i = 0; i < r; ++i) {
    for (int j = 0; j < r; ++j) {
      C(i, j) = H.col(i).dot(H.col(j));
    }
  }
  Matrix<Scalar, Dynamic, Dynamic, Options, Dynamic, Dynamic> D = C.inverse();
  for (int i = r; i < n; ++i) {
    // Step 5, Modify H_i
    Matrix<Scalar, Dynamic, 1, Options, Dynamic, 1> V = C.col(i);
    Matrix<Scalar, Dynamic, 1, Options, Dynamic, 1> X = D * V;
    for (int j = 0; j < r; ++j) {
      Scalar m_j = floor(X(j) + 0.5);
      H.col(i) -= m_j * H.col(j);
    }
  }
  // Step 4, Finished?
  return H;
}

#endif // ifndef INT_KER_IM_H
