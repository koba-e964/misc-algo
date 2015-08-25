#include "./lll.h"

int main(void) {
  MatrixXd basis(3, 3);
  basis << 1, -1, 3,
  1, 0, 5,
  1, 2, 6;
  MatrixXi basis_int(3, 3);
  basis_int << 1, -1, 3,
  1, 0, 5,
  1, 2, 6;
  MatrixXd h = lll_reduce(basis);
  for (int i = 0; i < 3; ++i) {
    cout << "basis[" << i << "]=" << basis.col(i) << endl;
  }
  cout << "H = " << h << endl;
  MatrixXi h2 = integral_lll_reduce(basis_int);
  cout << "H2 = " << h2 << endl;
}
