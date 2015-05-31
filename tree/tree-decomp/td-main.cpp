#include <iostream>
#include "./td.h"


using namespace std;
int main(void) {
  TreeDecomp td;
  cin >> td;
  cout << td << endl;
  cout << "width = " << td.width() << endl;
}
