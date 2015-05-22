#include "./td.cpp"

struct Graph {
  vector<vector<int> > adj; // n * n matrix
};

int max_indep(const TreeDecomp &td, const Graph &g) {
  return 0;
}

int main(void) {
  int n;
  TreeDecomp td;
  cin >> td;
  n = td.parent.size();
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
  vector<int> weight(n);
  for (int i = 0; i < n; ++i) { // read weights of vertices
    cin >> weight[i];
  }
}
