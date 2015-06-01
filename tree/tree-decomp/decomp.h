#ifndef DECOMP_H_
#define DECOMP_H_
#include "./td.h"
#include <vector>

/* Makes a path-decomposition of the given (undirected) graph.
 * The graph is given in adjacent list,
 * i.e. for every i, graph[i] is the list of vertices adjacent to i.
 * Vertices are labeled with 0 .. n-1.
 */
TreeDecomp path_decomp(const std::vector<std::vector<int> > & graph);

void make_nice_decomp_rec(const TreeDecomp &td, TreeDecomp &dest, int r);

TreeDecomp make_nice_decomp(const TreeDecomp & td);

#endif // #ifndef DECOMP_H_
