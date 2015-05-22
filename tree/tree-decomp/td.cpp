#include <bits/stdc++.h>


using namespace std;

struct TreeDecomp {
  vector<int> parent;
  vector<vector<int> > children;
  vector<vector<int> > bags;
  int wei;
  int width() const {
    return wei - 1;
  }
};


istream &operator>>(istream &is, TreeDecomp &td) {
  int n;
  cin >> n; // |V(T)|
  td.wei = 0;
  td.parent.resize(n,-1);
  td.children.resize(n, vector<int>());
  for (int i = 0; i < n; ++i) {
    cin >> td.parent[i];// parent[0] must be -1.
    int m;
    cin >> m;
    td.wei = max(td.wei, m);
    vector<int> &ch = td.children[i];
    ch.resize(m, -1);
    for (int j = 0; j < m; ++j) {
      cin >> ch[j];
    }
  }
  return is;
}

ostream &operator<<(ostream &os, const TreeDecomp &td) {
  cout << "*** Tree Decomposition ***" << endl;
  for (int i = 0; i < td.parent.size(); ++i) {
    cout << "parent[" << i << "] = " << td.parent[i] << endl;
    const vector<int> &ch = td.children[i];
    cout << "bags[" << i << "]:";
    for (int j = 0; j < ch.size(); ++j) {
      cout << ch[j] << " ";
    }
    cout << endl;
  }
  return os;
}


int main(void) {
  TreeDecomp td;
  cin >> td;
  cout << td << endl;
  cout << "width = " << td.width() << endl;
}
