#include <vector>
#include <iostream>

using namespace std;
int main(void) {
  int n;
  cin >> n;
  vector<int > graph; // adjacent matrix, bits
  for (int i = 0; i < n; i++) {
    int m;
    cin >> m;
    int b = 0;
    for (int j = 0; j < m; j++) {
      int t;
      cin >> t;
      b |= 1 << t;
    }
    graph.push_back(b);
  }
  vector<int> weight(n);
  for (int i = 0; i < n; i++) {
    cin >> weight[i];
  }
  // naive O(2^n * n)-time algorithm
  if (n > 20) {
    cerr << "n is too large:" << n << endl;
    return 1;
  }
  int ma = 0;
  for (int bits = 0 ; bits < (1 << n); ++bits) {
    int sum = 0;
    bool indep = true;
    for (int i = 0; i < n; ++i) {
      if ((bits & (1 << i)) == 0) {
	continue;
      }
      if (bits & graph[i]) { // not independent
	indep = false;
	break;
      }
      sum += weight[i];
    }
    if (indep) {
      ma = max(ma, sum);
    }
  }
  cout << "max-indep = " << ma << endl;
}
