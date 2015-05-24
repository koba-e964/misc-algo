#include "./td.cpp"

struct Graph {
  vector<vector<int> > adj; // n * n matrix
  vector<int> weight; // weights of vertices
};

const int MAX_WIDTH = 8;
const int MEMO = 1 << (MAX_WIDTH + 1);

// for nice tree decomposition
void rec(const TreeDecomp &td, const Graph &g, int r, int dp[][MEMO]) {
  int s = td.children[r].size();
  const vector<int> &bag = td.bags[r];
  if (s == 0) { // Leaf
    assert(bag.size() == 1);
    dp[r][0] = 0;
    dp[r][1] = g.weight[r];
    return;
  }
  for (int i = 0; i < s; ++i) {
    rec(td, g, td.children[r][i], dp);
  }
  if (s == 1) { // introduce, forget
    int b = bag.size();
    int ch = td.children[r][0];
    if (b - td.bags[ch].size() == 1) { // introduce
      cout << "intro: " << r << " <- " << ch << endl;
      return;
    }
    if (b - td.bags[ch].size() == -1) { // forget
      cout << "forget: " << r << " <- " << ch << endl;
      return;
    }
    
    for (int bits = 0; bits < (1 << b); ++bits) {
      dp[r][bits] = 0; // TODO;
    }
    return;
  }
  if (s == 2) { // join
    int b = bag.size();
    int ch1 = td.children[r][0];
    int ch2 = td.children[r][1];
    for (int bits = 0; bits < (1 << b); ++bits) {
      dp[r][bits] = max(dp[ch1][bits], dp[ch2][bits]);
    }
  }
}

int max_indep(const TreeDecomp &td, const Graph &g) {
  int n = td.size();
  if (td.width() > MAX_WIDTH) {
    assert(0);
  }
  int (*dp)[MEMO] = new int[n][MEMO];
  rec(td, g, 0, dp);
  int ma = 0;
  for (int bits = 0; bits < (1 << td.bags[0].size()); ++bits) {
    ma = max(ma, dp[0][bits]);
  }
  return ma;
}

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
