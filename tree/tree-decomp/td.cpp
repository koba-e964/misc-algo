#include "./td.h"
#include <vector>
#include <iostream>

using namespace std;

istream &operator>>(istream &is, TreeDecomp &td) {
  int n;
  cin >> n; // |V(T)|
  td.wei = 0;
  td.parent.resize(n,-1);
  td.children.resize(n, vector<int>());
  td.bags.resize(n, vector<int>());
  for (int i = 0; i < n; ++i) {
    cin >> td.parent[i];// parent[0] must be -1.
    if (td.parent[i] >= 0) {
      td.children[td.parent[i]].push_back(i);
    }
    int m;
    cin >> m;
    td.wei = max(td.wei, m);
    vector<int> &ch = td.bags[i];
    ch.resize(m, -1);
    for (int j = 0; j < m; ++j) {
      cin >> ch[j];
    }
  }
  return is;
}

ostream &operator<<(ostream &os, const TreeDecomp &td) {
  cout << "*** Tree Decomposition ***" << endl;
  for (int i = 0; i < (int)td.parent.size(); ++i) {
    cout << "[" << i << "] : " << td.parent[i];
    const vector<int> &ch = td.children[i];
    cout << "(";
    for (int j = 0; j < (int)ch.size(); ++j) {
      cout << ch[j] << " ";
    }
    cout << ") ";
    const vector<int> &bag = td.bags[i];
    cout << "{";
    for (int j = 0; j < (int)bag.size(); ++j) {
      cout << bag[j] << " ";
    }
    cout << "}" << endl;
  }
  cout << "tree-width = " << td.width() << endl;
  return os;
}

