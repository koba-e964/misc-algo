#include "./td.h"
#include "./decomp.h"
#include "./max-indep.h"
#include <vector>
#include <iostream>

using namespace std;
int main(void) {
  int n;
  cin >> n;
  vector<vector<int> > graph; // adjacent list representation
  for (int i = 0; i < n; i++) {
    int m;
    cin >> m;
    vector<int> adj(m);
    for (int j = 0; j < m; j++) {
      cin >> adj[j];
    }
    graph.push_back(adj);
  }
  TreeDecomp td = greedy_degree(graph);
  TreeDecomp nicetd = make_nice_decomp(td);
  Graph g;
  /* convert to matrix representation */
  g.adj.resize(n, vector<int>());
  for (int i = 0; i < n; ++i) {
    g.adj[i].resize(n, 0);
  }
  for (int i = 0; i < n; ++i) { // read a graph
    int m = graph[i].size();
    for (int j = 0; j < m; ++j) {
      int t = graph[i][j];
      g.adj[i][t] = 1;
      g.adj[t][i] = 1;
    }
  }
  g.weight.resize(n);
  for (int i = 0; i < n; ++i) {
    cin >> g.weight[i];
  }
  int mitd = max_indep(nicetd, g);
  cout << "max-indep(td) = " << mitd << " (tree-width = " << nicetd.width() << ")" << endl;
}
