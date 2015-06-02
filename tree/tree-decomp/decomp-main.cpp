#include "./td.h"
#include "./decomp.h"
#include <iostream>

using namespace std;
int main(void) {
  int n;
  cin >> n;
  vector<vector<int> > graph;
  for (int i = 0; i < n; i++) {
    int m;
    cin >> m;
    vector<int> adj(m);
    for (int j = 0; j < m; j++) {
      cin >> adj[j];
    }
    graph.push_back(adj);
  }
  TreeDecomp pd = path_decomp(graph);
  cout << pd << endl;
  TreeDecomp nice = make_nice_decomp(pd);
  cout << nice << endl;
  TreeDecomp td = greedy_degree(graph);
  cout << td << endl;
  TreeDecomp nicetd = make_nice_decomp(td);
  cout << nicetd << endl;
  
}
