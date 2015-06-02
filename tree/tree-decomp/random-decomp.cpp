#include "./td.h"
#include "./decomp.h"
#include <vector>
#include <iostream>
#include <cstdlib>

const int N = 100;

int g[N][N];

using namespace std;

int main() {
  int n;
  cin >> n;
  srand(time(0));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < N; ++j) {
      for (int k = 0; k < N; ++k) {
	g[j][k] = 0;
	if (j != k && (rand() % 100) <= 19) { // probability 0.2
	  g[j][k] = 1;
	}
      }
    }
    vector<vector<int> > adj(N);
    int e = 0;
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
	if (g[i][j]) {
	  adj[i].push_back(j);
	}
      }
      e += adj[i].size();
    }
    cout << "|V| = " << N << ", |E| = " << e << endl;
    cout << "tw(path-decomposition):" << path_decomp(adj).width() << endl;
    cout << "tw(greedy-degree):" << greedy_degree(adj).width() << endl;
  }
}
