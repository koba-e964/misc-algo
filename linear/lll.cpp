#include <vector>
#include <iostream>

#include "Eigen/Core"
using namespace std;
using namespace Eigen;


/*
 * From CiCANT
 */

void red(int k, int l, MatrixXd &mu, vector<VectorXd> &basis) {
  if (mu(k, l) <= 0.5) {
    return;
  }
  double q = floor(mu(k, l) + 0.5);
  basis[k] -= q * basis[l];
  mu(k, l) -= q;
  for (int i = 0; i < l; ++i) {
    mu(k, i) -= q * mu(l, i);
  }
}

void swap(int k, vector<VectorXd> &basis, MatrixXd &mu, vector<double> &cb, int kmax) {
  swap(basis[k], basis[k - 1]);
  for (int j = 0; j < k - 1; ++j) {
    swap(mu(k, j), mu(k - 1, j));
  }
  double m = mu(k, k - 1), b = cb[k] + m * m * cb[k - 1];
  mu(k, k - 1) = m * cb[k - 1] / b;
  cb[k] *= cb[k - 1] / b;
  cb[k - 1] = b;
  for (int i = k + 1; i <= kmax; ++i) {
    double t = mu(i, k);
    mu(i, k) = mu(i, k - 1) - m * t;
    mu(i, k - 1) = t + mu(k, k - 1) * mu(i, k);
  }
}


void lll_reduce(vector<VectorXd> &basis) {
  int k = 1, kmax = 0;
  int n = basis.size();
  vector<VectorXd> bc(n); // b*
  bc[0] = basis[0];
  vector<double> cb(n); // B
  cb[0] = bc[0].squaredNorm();
  MatrixXd mu(n, n);
  // Step 2
  do {
    if (k > kmax) {
      kmax = k;
      bc[k] = basis[k];
      for (int j = 0; j < k; ++j) {
	mu(k, j) = basis[k].dot(bc[j]) / cb[j];
	bc[k] -= mu(k, j) * bc[j];
      }
      cb[k] = bc[k].squaredNorm();
      if (cb[k] == 0) {
	cerr << "basis did not form a basis" << endl;
	abort();
      }
    }
    //Step 3
    while (1) {
      red(k, k - 1, mu, basis);
      if (cb[k] < (0.75 - pow(mu(k, k - 1), 2.0)) * cb[k - 1]) {
	swap(k, basis, mu, cb, kmax);
	k = max(1, k - 1);
	continue;
      }
      break;
    }
    for (int l = k - 2; l >= 0; --l) {
      red(k, l, mu, basis);
    }
    ++k;
  } while (k < n);
  return;
}

int main(void) {
  VectorXd b1(3), b2(3), b3(3);
  b1 << 1, 1, 1;
  b2 << -1, 0, 2;
  b3 << 3, 5, 6;
  vector<VectorXd> basis;
  basis.push_back(b1);
  basis.push_back(b2);
  basis.push_back(b3);
  lll_reduce(basis);
  for (int i = 0; i < 3; ++i) {
    cout << "basis[" << i << "]=" << basis[i] << endl;
  }
}
