#include "./td.h"
#include "./decomp.h"
#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>

using namespace std;

/* Makes a path-decomposition of the given (undirected) graph.
 * The graph is given in adjacent list,
 * i.e. for every i, graph[i] is the list of vertices adjacent to i.
 * Vertices are labeled with 0 .. n-1.
 */
TreeDecomp path_decomp(const vector<vector<int> > & graph) {
  int n = graph.size();
  TreeDecomp td;
  td.parent.resize(n);
  for (int i = 0; i < n; i++) {
    td.parent[i] = i - 1;
  }
  td.children.resize(n);
  for (int i = 0; i < n - 1; i++) {
    td.children[i].push_back(i + 1);
  }
  td.bags.resize(n);
  int w = 0;
  for (int i = 0; i < n; ++i) {
    vector<int> bag;
    bag.push_back(i);
    for (int j = i + 1; j < n; ++j) {
      int inc = 0;
      for (int k = 0; k < (int)graph[j].size(); ++k) {
	inc = inc || (graph[j][k] <= i);
      }
      if (inc) {
	bag.push_back(j);
      }
    }
    td.bags[i] = bag;
    w = max(w, (int)bag.size());
  }
  td.wei = w;
  return td;
     
}


/*
 * Assumes that bags are already sorted.
 * Returns the index of vertex r in dest.
 */
int make_nice_decomp_rec(const TreeDecomp &td, TreeDecomp &dest, int r, int parent) {
  // since we handle with path decompositions only, each vertex has at most one child.
  int rd = dest.size();
  int nc = td.children[r].size();
  if (nc == 0) {
    // No child. This node will be converted to a leaf.
    vector<int> bag = td.bags[r];
    int bs = bag.size();
    // nodes rd .. rd + bs are used.
    dest.parent.resize(rd + bs + 1);
    dest.children.resize(rd + bs + 1);
    dest.bags.resize(rd + bs + 1);
    for (int i = 0; i <= bs; ++i) {
      dest.parent[rd + i] = i == 0 ? parent : rd + i - 1; // assume that this decomposition is a path-decomp. in general situation, we have to calc the parent.
      if (i < bs) {
	dest.children[rd + i].push_back(rd + i + 1);
      }
      dest.bags[rd + i] = bag;
      bag.pop_back(); // introduce
    }
    return rd;
  }
  int cur_root = rd;
  int cur_par = parent;
  for (int chid = 0; chid < nc; ++chid) {
    vector<int> pb = td.bags[r]; // mutable
    int child = td.children[r][chid];
    const vector<int> cb = td.bags[child];
    vector<vector<int> > dbags;
    dbags.push_back(pb); // top
    if (chid < nc - 1) { // join
      dbags.push_back(pb);
    }
    while (1) { // until pb <= cb
      bool cont = true;
      int r = -1; // dummy
      for (int i = 0; cont && i < (int)pb.size(); ++i) { // O(|pb| * |cb|)-time
	if (count(cb.begin(), cb.end(), pb[i]) == 0) {
	  r = i;
	  cont = false;
	}
      }
      if (cont) { // pb <= cb
	break;
      }
      // (pb - cb) contains pb[r]
      pb.erase(pb.begin() + r);
      dbags.push_back(pb);
    }
    for (size_t s = pb.size(); s < cb.size(); ++s) {
      int r = -1; // dummy
      for (int i = 0; i < (int)cb.size(); ++i) { // O(|pb| * |cb|)-time
	if (count(pb.begin(), pb.end(), cb[i]) == 0) {
	  r = i;
	  break;
	}
      }
      // (cb - pb) contains cb[r]
      vector<int>::iterator it = upper_bound(pb.begin(), pb.end(), cb[r]);
      pb.insert(it, cb[r]);
      dbags.push_back(pb);
    }
    int ds = dbags.size();
    // nodes cur_root .. cur_root + ds - 1 are used.
    int end = cur_root + ds - 1;
    dest.parent.resize(end);
    dest.children.resize(end);
    dest.bags.resize(end);
    for (int i = 0; i < ds; ++i) {
      if (i < ds - 1) { // if i == ds - 1, creation of node rd + i will be done by subroutine.
	dest.parent[cur_root + i] = i == 0 ? cur_par : cur_root + i - 1; // assume that this decomposition is a path-decomp. in general situation, we have to calc the parent.
	dest.children[cur_root + i].push_back(cur_root + i + 1);
	dest.bags[cur_root + i] = dbags[i];
      }
    }
    int child_dest = make_nice_decomp_rec(td, dest, child, cur_root + ds - 2);
    dest.children[end - 1][dest.children[end - 1].size() - 1] = child_dest;
    if (chid < nc - 1) {
      cur_par = cur_root;
      cur_root = dest.size();
      dest.children[cur_par].push_back(cur_root);
    }
  }
  return rd;
}

TreeDecomp make_nice_decomp(const TreeDecomp & td) {
  TreeDecomp dest;
  make_nice_decomp_rec(td, dest, 0, -1);
  int w = 0;
  for (int i = 0; i < dest.size(); ++i) {
    w = max(w, (int)dest.bags[i].size());
  }
  dest.wei = w;
  return dest;
}



TreeDecomp greedy_degree(const vector<vector<int> > &graph) {
  int n = graph.size(); // |V|
  vector<vector<bool> > copy(n); // matrix
  for(int i = 0; i < n; ++i) {
    copy[i].resize(n);
    for (int j = 0; j < (int)graph[i].size(); ++j) {
      copy[i][graph[i][j]] = true;
    }
  }
  vector<bool> used(n);
  vector<int> degree(n);
  for (int i = 0; i < n; ++i) {
    degree[i] = graph[i].size();
  }
  vector<int> elim(n, -1);
  vector<vector<int> > bag(n, vector<int>());
  for (int trial = 0; trial < n; trial++) {
    int mi = -1;
    for (int j = 0; j < n; j++) {
      if (used[j]) {
	continue;
      }
      if (mi == -1 || degree[mi] > degree[j]) {
	mi = j;
      }
    }
    elim[trial] = mi;
    vector<int> cb;
    cb.push_back(mi);
    for (int i = 0; i < n; ++i) {
      if (copy[mi][i] && ! used[i]) {
	cb.push_back(i);
      }
    }
    sort(cb.begin(), cb.end());
    bag[trial] = cb;
    for (int i = 0; i < (int)cb.size(); ++i) { // make N_G(mi) clique
      for (int j = 0; j < (int)cb.size(); ++j) {
	if (i == j) {
	  continue;
	}
	degree[cb[i]] += ! copy[cb[i]][cb[j]];
	copy[cb[i]][cb[j]] = true;
      }
    }
    for (int i = 0; i < n; ++i) { // remove vertex (mi)
      degree[mi] -= copy[mi][i];
      degree[i] -= copy[i][mi];
      copy[mi][i] = false;
      copy[i][mi] = false;
    }
    used[mi] = true;
  }
  TreeDecomp td;
  td.parent.resize(n);
  td.children.resize(n);
  td.bags.resize(n);
  for (int i = n - 1; i >= 0; --i) { // handle with elim[i].
    int tv = n - 1 - i;
    int par;
    if (tv == 0) {
      par = -1;
    } else {
      for (int j = i + 1; j < n; ++j) {
	if (count(bag[i].begin(), bag[i].end(), elim[j])) {
	  par = n - 1 - j;
	  break;
	}
      }
    }
    td.parent[tv] = par;
    td.children[par].push_back(tv);
    td.bags[tv] = bag[i];
  }
  int w = 0;
  for (int i = 0; i < td.size(); ++i) {
    w = max(w, (int)td.bags[i].size());
  }
  td.wei = w;
  return td;
}
