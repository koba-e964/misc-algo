#include "./lll.h"
#include "./int_ker_im.h"

void test_ker_im(void) {
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
  MatrixXd h = ker_im(basis, p, 1e-9);
  for (int i = 0; i < 3; ++i) {
    cout << "basis[" << i << "]=" << basis.col(i) << endl;
  }
  cout << "p = " << p << endl;
  cout << "H = " << h << endl;
}

int main(void) {
  test_ker_im();
}
