#include "./lll.h"

void test_dependent(void) {
  cout << "** test_dependent **" << endl;
  MatrixXd basis(3, 3);
  basis << 1, 2, 3,
    1, 1, 2,
    3, 0, 3;
  /*  basis << 1,2,0,
    1,1,0,
    3,0,0;
  */
  int p;
  MatrixXd h = dependent_lll_reduce(basis, p, 1e-9);
  for (int i = 0; i < 3; ++i) {
    cout << "basis[" << i << "]=" << basis.col(i) << endl;
  }
  cout << "p = " << p << endl;
  cout << "H = " << h << endl;
}


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
  test_dependent();
}
