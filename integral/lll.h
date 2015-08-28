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
  MS h(MS::Identity(n, n));
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

template<typename Scalar, int Cols, int Options, int MaxCols>
void integral_red(int k,
		  int l,
		  Matrix<Scalar, Cols, Cols, Options, MaxCols, MaxCols> &lambda,
		  vector<Scalar> &d,
		  Matrix<Scalar, Cols, Cols, Options, MaxCols, MaxCols> &h) {
  if (abs(lambda(k, l) * 2) <= d[l + 1]) {
    return;
  }
  Scalar q = (2 * lambda(k, l) + d[l + 1]) / (2 * d[l + 1]); // nearest integer to lambda(k, l) / d[l + 1]
  h.col(k) -= q * h.col(l);
  lambda(k, l) -= q * d[l + 1];
  for (int i = 0; i < l; ++i) {
    lambda(k, i) -= q * lambda(l, i);
  }
}


template<typename Scalar, int Cols, int Options, int MaxCols>
void integral_swap(int k,
		   Matrix<Scalar, Cols, Cols, Options, MaxCols, MaxCols> &lambda,
		   vector<Scalar> &d,
		   int kmax,
		   Matrix<Scalar, Cols, Cols, Options, MaxCols, MaxCols> &h) {
  int n = h.cols();
  for (int i = 0; i < n; i++) {
    swap(h(i, k), h(i, k - 1));
  }
  for (int j = 0; j < k - 1; ++j) {
    swap(lambda(k, j), lambda(k - 1, j));
  }
  Scalar lam = lambda(k, k - 1);
  Scalar B = (d[k - 1] * d[k + 1] + lam * lam) / d[k];
  for (int i = k + 1; i <= kmax; ++i) {
    Scalar t = lambda(i, k);
    lambda(i, k) = (d[k + 1] * lambda(i, k - 1) - lam * t) / d[k];
    lambda(i, k - 1) = t + lam * lambda(i, k) / B;
  }
  d[k] = B;
}

template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
Matrix<Scalar, Cols, Cols, Options, MaxCols, MaxCols>
integral_lll_reduce(const Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &basis) {
  int k = 1, kmax = 0;
  int n = basis.cols(); // b[0] ... b[n - 1]
  int m = basis.rows(); // dim b[i]
  vector<Scalar> d(n + 1); // d[i] corresponds to d_i in the book
  d[0] = 1;
  d[1] = basis.col(0).squaredNorm();
  typedef Matrix<Scalar, Cols, Cols, Options, MaxCols, MaxCols> MS;
  MS lambda(n, n);
  MS h(MS::Identity(n, n));
  // Step 2
  do {
    if (k > kmax) {
      kmax = k;
      for (int j = 0; j <= k; ++j) {
	Scalar u = basis.col(k).dot(basis.col(j));
	for (int i = 0; i < j; ++i) {
	  u = d[i + 1] * u - lambda(k, i) * lambda(j, i);
	  u /= d[i];
	}
	if (j < k) {
	  lambda(k, j) = u;
	} else {
	  d[k + 1] = u;
	}
      }
      if (d[k + 1] == 0) {
	cerr << "basis did not form a basis" << endl;
	abort();
      }
    }
    //Step 3
    while (1) {
      integral_red(k, k - 1, lambda, d, h);
      if (4 * d[k + 1] * d[k - 1] < 3 * d[k] * d[k] - lambda(k, k - 1) * lambda(k, k - 1)) {
	integral_swap(k, lambda, d, kmax, h);
	k = max(1, k - 1);
	continue;
      }
      break;
    }
    for (int l = k - 2; l >= 0; --l) {
      integral_red(k, l, lambda, d, h);
    }
    ++k;
  } while (k < n);
  return h;
}

template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
  void dependent_swap(int k, Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &basis,
		      Matrix<Scalar, Cols, Cols, Options, MaxCols, MaxCols> &mu,
		      vector<Scalar> &cb,
		      int kmax,
		      Matrix<Scalar, Cols, Cols, Options, MaxCols, MaxCols> &h,
		      Scalar eps) {
  for (int i = 0; i < mu.cols(); ++i) {
    swap(basis(i, k), basis(i, k - 1));
    swap(h(i, k), h(i, k - 1));
  }
  for (int j = 0; j < k - 1; ++j) {
    swap(mu(k, j), mu(k - 1, j));
  }
  Scalar m = mu(k, k - 1);
  Scalar b = cb[k] + m * m * cb[k - 1];
  if (b < eps) {
    swap(cb[k], cb[k - 1]);
    for (int i = k + 1; i <= kmax; ++i) {
      swap(mu(i, k), mu(i, k - 1));
    }
  } else if (cb[k] < eps && max(m, -m) > eps) {
    cout << "B_k = 0, \\mu != 0" << endl;
    cb[k - 1] = b;
    mu(k, k - 1) = 1 / m;
    for (int i = k + 1; i <= kmax; ++i) {
      mu(i, k - 1) /= m;
    }
  } else {
    Scalar t = cb[k - 1] / b;
    mu(k, k - 1) = m * t;
    cb[k] *= t;
    cb[k - 1] = b;
    for (int i = k + 1; i <= kmax; ++i) {
      t = mu(i, k);
      mu(i, k) = mu(i, k - 1) - m * t;
      mu(i, k - 1) = t + mu(k, k - 1) * mu(i, k);
    }
  }
}

template<typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
  Matrix<Scalar, Cols, Cols, Options, MaxCols, MaxCols>
  dependent_lll_reduce(Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> &basis, int &p, Scalar eps) {
  int k = 1, kmax = 0;
  int n = basis.cols();
  int m = basis.rows(); // dim b[i]
  Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols> bc(m, n); // b*
  bc.col(0) = basis.col(0);
  vector<Scalar> cb(n); // B
  cb[0] = bc.col(0).squaredNorm();
  typedef Matrix<Scalar, Cols, Cols, Options, MaxCols, MaxCols> MS;
  MS mu(n, n);
  MS h(MS::Identity(n, n));
  // Step 2
  do {
    if (1) { // TODO debug code
      cout << "** dump **" << endl;
      cout << "k = " << k << ", kmax = " << kmax << endl;
      cout << "H = " << h << endl;
      cout << "basis = " << basis << endl;
      cout << "b* = " << bc << endl;
      cout << "mu = " << mu << endl;
      cout << "B[] = {";
      for (int i = 0; i < n; ++i) {
	cout << cb[i] << ", ";
      }
      cout << "}" << endl;
    }
    if (k > kmax) {
      kmax = k;
      bc.col(0) = basis.col(0);
      bc.col(k) = basis.col(k);
      for (int j = 0; j < k; ++j) {
	if (cb[j] >= eps) {
	  mu(k, j) = basis.col(k).dot(bc.col(j)) / cb[j];
	} else {
	  mu(k,j) = 0;
	}
	bc.col(k) -= mu(k, j) * bc.col(j);
      }
      cout << "bc.col(" << k << ") = " << bc.col(k) << endl;
      cb[k] = bc.col(k).squaredNorm();
    }
    //Step 3
    bool swapped = 0;
    while (1) {
      red(k, k - 1, mu, basis, h);
      if (cb[k] < (0.75 - pow(mu(k, k - 1), 2.0)) * cb[k - 1]) {
	dependent_swap(k, basis, mu, cb, kmax, h, eps);
	k = max(1, k - 1);
	swapped = 1; //TODO additional
	continue;
      }
      break;
    }
    for (int l = k - 2; l >= 0; --l) {
      red(k, l, mu, basis, h);
    }
    if (swapped) {
      kmax = k -1;
    } else {
      ++k;
    }
  } while (k < n);
  int r = 0;
  for (int i = 0; i < n; i++) {
    r += basis.col(i).squaredNorm() < eps ? 1 : 0;
  }
  p = n - r; // output p
  return h;
}
