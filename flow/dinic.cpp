#include <iostream>
#include <vector>
#include <queue>

class Dinic {
private:
  struct edge {
    int to, cap, rev; // rev is the position of reverse edge in graph[to]
  };
  std::vector<std::vector<edge> > graph;
  std::vector<int> level;
  std::vector<int> iter;
  /* Perform bfs and calculate distance from s */
  void bfs(int s) {
    level.assign(level.size(), -1);
    std::queue<int> que;
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
	int newf = f == -1 ? e.cap : std::min(f, e.cap);
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
public:
  /* v is the number of vertices (labeled from 0 .. v-1) */
  Dinic(int v) : graph(v), level(v, -1), iter(v, 0) {}
  void add_edge(int from, int to, int cap) {
    graph[from].push_back((edge) {to, cap, graph[to].size()});
    graph[to].push_back((edge) {from, 0, graph[from].size() - 1});
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

int main(void){
  using namespace std;
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
