#include "./td.h"
#include "./max-indep.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;


const int MAX_WIDTH = 16;
const int MEMO = 1 << (MAX_WIDTH + 1);
const int minf = -123456789;
const bool DEBUG = false;

// for nice tree decomposition
void rec(const TreeDecomp &td, const Graph &g, int r, int dp[][MEMO]) {
  int s = td.children[r].size();
  const vector<int> &bag = td.bags[r];
  if (s == 0) { // Leaf
    assert(bag.size() == 0);
    dp[r][0] = 0;
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
	if (dp[r][bits]  <= minf) {
	  dp[r][bits] = minf;
	}
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
      if (dp[r][bits] <= minf) {
	dp[r][bits] = minf;
      }
    }
    return;
  }
  assert (! "It is not a nice tree decomposition. |Children| >= 3.");
}

int max_indep(const TreeDecomp &td, const Graph &g) {
  int n = td.size();
  if (td.width() > MAX_WIDTH) {
    assert(! "tree-width too large");
  }
  int (*dp)[MEMO] = new int[n][MEMO];
  rec(td, g, 0, dp);
  int ma = 0;
  if (DEBUG) {
    for (int r = 0; r < n; ++r) {
      for (int bits = 0; bits < (1 << td.bags[r].size()); ++bits) {
	cout << "dp[" << r << "][" << bits << "] = ";
	if (dp[r][bits] == minf) {
	  cout << "- inf"  << endl;
	} else {
	  cout << dp[r][bits] << endl;
	}
      }
    }
  }
  for (int bits = 0; bits < (1 << td.bags[0].size()); ++bits) {
    ma = max(ma, dp[0][bits]);
  }
  return ma;
}

