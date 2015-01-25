#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <queue>

struct edge {
  int to, cap, rev; // rev is the position of reverse edge in graph[to]
};

  using namespace std;
class Dinic {
private:
  vector<vector<edge> > graph;
  vector<int> level;
  vector<int> iter;
public:
  Dinic(int v) : graph(v), level(v, -1), iter(v, 0) {}
  void add_edge(int from, int to, int cap) {
    graph[from].push_back((edge) {to, cap, graph[to].size()});
    graph[to].push_back((edge) {from, 0, graph[from].size() - 1});
  }
  /* Perform bfs and calculate distance from s */
  void bfs(int s) {
    level.assign(level.size(), -1);
    queue<int> que;
    level[s] = 0;
    que.push(s);
    while (! que.empty()) {
      int v = que.front(); que.pop();
      for (int i = 0; i < graph[v].size(); ++i) {
	edge &e = graph[v][i];
	if (e.cap > 0 && level[e.to] == -1) {
	  level[e.to] = level[v] + 1;
	  que.push(e.to);
	}
      }
    }
  }
  /* search augment path by dfs.
   if f == -1, f is treated as infinity. */
  int dfs(int v, int t, int f) {
    if (v == t) {
      return f;
    }
    for (int &i = iter[v]; i < graph[v].size(); ++i) {
      edge &e = graph[v][i];
      if (e.cap > 0 && level[v] < level[e.to]) {
	int newf = f == -1 ? e.cap : min(f, e.cap);
	int d = dfs(e.to, t, newf);
	if (d > 0) {
	  e.cap -= d;
	  graph[e.to][e.rev].cap += d;
	  return d;
	}
      }
    }
    return 0;
  }
  int max_flow(int s, int t) {
    int flow = 0;
    while (1) {
      bfs(s);
      if (level[t] < 0) {
	return flow;
      }
      iter.assign(iter.size(), 0);
      int f;
      while ((f = dfs(s, t, -1)) > 0) {
	flow += f;
      }
    }
  }
};


#define REP(i,s,n) for(int i=(int)(s);i<(int)(n);i++)

using namespace std;
typedef long long int ll;
const double EPS=1e-9;



int main(void){
  Dinic din(6);
  int edges[8][3] = {
    {0,1,3},
    {0,2,3},
    {1,2,2},
    {1,3,3},
    {2,4,2},
    {3,4,4},
    {3,5,2},
    {4,5,3},
  }; // Example from wikipedia (https://en.wikipedia.org/wiki/Maximum_flow_problem)
  for (int i = 0; i < 8; ++i) {
    din.add_edge(edges[i][0], edges[i][1], edges[i][2]);
  }
  cout << din.max_flow(0, 5) << endl;
}
