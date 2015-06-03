#include "./td.h"
#include "./decomp.h"
#include <vector>
#include <iostream>
#include <cstdlib>

const int M = 1000;

int g[M][M];

using namespace std;

int main() {
  int n, N, E;
  cin >> n >> N >> E;
  srand(time(0));
  double avpw = 0, avtw = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < N; ++j) {
      for (int k = 0; k < N; ++k) {
	g[j][k] = 0;
	if (j != k && (rand() % (N * N - 1)) < 2 * E) { // probability 2E / N(N-1)
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
    int pw = path_decomp(adj).width();
    int tw = greedy_degree(adj).width();
    cout << "|V| = " << N << ", |E| = " << e << endl;
    cout << "tw(path-decomposition):" << pw << endl;
    cout << "tw(greedy-degree):" << tw << endl;
    avpw += pw;
    avtw += tw;
  }
  cout << "averate tw(path) = " << avpw / n << endl;
  cout << "averate tw(greedy) = " << avtw / n << endl;
}
