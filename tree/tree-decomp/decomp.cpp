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
 */
void make_nice_decomp_rec(const TreeDecomp &td, TreeDecomp &dest, int r) {
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
      dest.parent[rd + i] = rd + i - 1; // assume that this decomposition is a path-decomp. in general situation, we have to calc the parent.
      if (i < bs) {
	dest.children[rd + i].push_back(rd + i + 1);
      }
      dest.bags[rd + i] = bag;
      bag.pop_back(); // introduce
    }
    return;
  }
  if (nc == 1) { // one child.
    vector<int> pb = td.bags[r]; // mutable
    int child = td.children[r][0];
    const vector<int> cb = td.bags[child];
    vector<vector<int> > dbags;
    dbags.push_back(pb); // top
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
    // nodes rb .. rb + ds - 1 are used.
    dest.parent.resize(rd + ds - 1);
    dest.children.resize(rd + ds - 1);
    dest.bags.resize(rd + ds - 1);
    for (int i = 0; i < ds; ++i) {
      if (i < ds - 1) { // if i == ds - 1, creation of node rd + i will be done by subroutine.
	dest.parent[rd + i] = rd + i - 1; // assume that this decomposition is a path-decomp. in general situation, we have to calc the parent.
	dest.children[rd + i].push_back(rd + i + 1);
	dest.bags[rd + i] = dbags[i];
      }
    }
    make_nice_decomp_rec(td, dest, child);
    return;
    
  }
  assert (!"not a path-decomposition. detected branch");
}

TreeDecomp make_nice_decomp(const TreeDecomp & td) {
  TreeDecomp dest;
  make_nice_decomp_rec(td, dest, 0);
  int w = 0;
  for (int i = 0; i < dest.size(); ++i) {
    w = max(w, (int)dest.bags[i].size());
  }
  dest.wei = w;
  return dest;
}
