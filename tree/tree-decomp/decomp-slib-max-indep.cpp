#include "./td.h"
#include "./decomp.h"
#include "./max-indep.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <cassert>

using namespace std;

void stoupper(std::string &s) {
  for (int i = 0; i < (int) s.length(); ++i) {
    s[i] = toupper(s[i]);
  }
}


string comment(void) {
  string s, name;
  while (cin >> s) {
    stoupper(s);
    if (s == "END") {
      return name;
    }
    if (s == "NAME") {
      cin >> name;
    }
  }
  return "<unnamed>";
}

vector<vector<int> > read_stp_graph(int &n, int &m) {
  string s;
  cin >> s;
  stoupper(s);
  assert (s == "NODES");
  cin >> n;
  cin >> s;
  stoupper(s);
  assert (s == "EDGES");
  cin >> m;
  vector<vector<int> > graph(n); // adjacent list representation
  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> s;
    stoupper(s);
    assert (s == "E");
    cin >> u >> v >> s;
    u--;
    v--;
    graph[u].push_back(v);
    graph[v].push_back(u);
  }
  for (int i = 0; i < n; ++i) {
    sort(graph[i].begin(), graph[i].end());
  }
  return graph;
}

int main(void) {
  int n = 0, m = 0;
  string s;
  vector<vector<int> > graph;
  string name;
  getline(cin, s);
  while (1) {
    cin >> s;
    stoupper(s);
    assert (s == "SECTION");
    cin >> s;
    stoupper(s);
    if (s == "COMMENT") {
      name = comment();
      continue;
    }
    if (s == "GRAPH") {
      graph = read_stp_graph(n, m);
      break;
    }
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
    g.weight[i] = 1; // weight is 1
  }
  cout << "graph name : " << name << ", |V| = " << n << ", |E| = " << m << endl;
  cout << "tree-width = " << nicetd.width() << endl;
  int mitd = max_indep(nicetd, g);
  cout << "max-indep(td) = " << mitd << endl;
  cout << name.substr(1, name.length() - 2) << " & " << n << " & "
       << m << " & " << nicetd.width() << " & " << mitd << "\\\\" << endl;
}
