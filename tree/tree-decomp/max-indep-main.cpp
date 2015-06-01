#include "./td.h"
#include "./max-indep.h"
#include <iostream>
#include <vector>
using namespace std;
int main(void) {
  int n;
  TreeDecomp td;
  cin >> td;
  cout << td;
  cin >> n; // |V(G)|
  Graph g;
  g.adj.resize(n, vector<int>());
  for (int i = 0; i < n; ++i) {
    g.adj[i].resize(n, 0);
  }
  for (int i = 0; i < n; ++i) { // read a graph
    int m;
    cin >> m;
    for (int j = 0; j < m; ++j) {
      int t;
      cin >> t;
      g.adj[i][t] = 1;
      g.adj[t][i] = 1;
    }
  }
  g.weight.resize(n);
  for (int i = 0; i < n; ++i) { // read weights of vertices
    cin >> g.weight[i];
  }
  cout << "max-indep = " << max_indep(td, g) << endl;
}
