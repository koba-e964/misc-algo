#include "./td.cpp"

struct Graph {
  vector<vector<int> > adj; // n * n matrix
  vector<int> weight; // weights of vertices
};

const int MAX_WIDTH = 8;
const int MEMO = 1 << (MAX_WIDTH + 1);
const int minf = -123456789;

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
    size_t b = bag.size();
    int ch = td.children[r][0];
    const vector<int> &chb = td.bags[ch];
    if (b == chb.size() + 1) { // introduce
      size_t v, vpos; // v : the index of introduced vertex, bag[vpos] ==  v
      vector<size_t> map(b); // map from bag to chb. bag[i] = chb[map[i]]
      for (size_t i = 0; i < b; ++i) {
	vector<int>::const_iterator it = find(chb.begin(), chb.end(), bag[i]);
	if (it == chb.end()) {
	  v = bag[i];
	  vpos = i;
       	}
        map[i] = it - chb.begin();
      }
      const vector<int> &adj = g.adj[v];
      int adjbits = 0; // S /\ B_ch in B_ch
      for (size_t i = 0; i < b; i++) {
	if (adj[bag[i]]) {
	  adjbits |= 1 << i;
	}
      }
      for (int bits = 0; bits < (1 << b); ++bits) {
	int sum = 0;
	if (bits & (1 << vpos)) {
	  if (bits & adjbits) {
	    dp[r][bits] = minf;
	    continue;
	  }
	  sum += g.weight[v];
	}
	int chbits = 0;
	for (size_t i = 0; i < b; ++i) {
	  if ((bits & (1 << i)) == 0) { continue; }
	  if (map[i] == b - 1) { continue; }
	  chbits |= 1 << map[i];
	}
	dp[r][bits] = dp[ch][chbits] + sum;
      }
      return;
    }
    if (b == chb.size() - 1) { // forget
      int v; // v : 1 << the index of forgotten vertex
      vector<int> map(b); // map from bag to chb. bag[i] = chb[map[i]]
      v = (1 << (b + 1)) - 1;
      for (size_t i = 0; i < b; ++i) {
	int t = find(chb.begin(), chb.end(), bag[i]) - chb.begin();
	map[i] = t;
	v ^= 1 << t;
      }
      assert (v != 0 && (v & (-v)) == v); // one bit is activated
      for (int bits = 0; bits < (1 << b); ++bits) {
	int chbits = 0;
	for (size_t i = 0; i < b; ++i) {
	  if ((bits & (1 << i)) == 0) { continue; }
	  chbits |= 1 << map[i];
	}
	dp[r][bits] = max(dp[ch][chbits], dp[ch][chbits | v]);
      }
      return;
    }
  }
  if (s == 2) { // join
    size_t b = bag.size();
    int ch1 = td.children[r][0];
    int ch2 = td.children[r][1];
    assert (bag == td.bags[ch1] && bag == td.bags[ch2]);
    for (int bits = 0; bits < (1 << b); ++bits) {
      dp[r][bits] = dp[ch1][bits] + dp[ch2][bits];
      for (size_t i = 0; i < b; i++) {
	if ((bits & (1 << i)) == 0) {
	  continue;
	}
	int t = bag[i];
	dp[r][bits] -= g.weight[t];
      }
    }
    return;
  }
  assert (! "It is not a nice tree decomposition. |Children| >= 3.");
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
