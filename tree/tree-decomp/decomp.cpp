#include "./td.h"
#include <vector>
#include <iostream>

using namespace std;

/* Makes a path-decomposition of the given (undirected) graph.
 * The graph is given in adjacent list,
 * i.e. for every i, graph[i] is the list of vertices adjacent to i.
 * Vertices are labeled with 0 .. n-1.
 */
TreeDecomp path_decomp(const vector<vector<int> > & graph) {
  int n = graph.size();
  TreeDecomp td;
  td.parent.resize(n);
  for (int i = 0; i < n; i++) {
    td.parent[i] = i - 1;
  }
  td.children.resize(n);
  for (int i = 0; i < n - 1; i++) {
    td.children[i].push_back(i + 1);
  }
  td.bags.resize(n);
  for (int i = 0; i < n; ++i) {
    vector<int> bag;
    bag.push_back(i);
    for (int j = i + 1; j < n; ++j) {
      int inc = 0;
      for (int k = 0; k < (int)graph[j].size(); ++k) {
	inc = inc || (graph[j][k] <= i);
      }
      if (inc) {
	bag.push_back(j);
      }
    }
    td.bags[i] = bag;
  }
  return td;
     
}

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
  TreeDecomp td = path_decomp(graph);
  cout << td << endl;
}
