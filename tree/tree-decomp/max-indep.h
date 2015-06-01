#ifndef MAX_INDEP_H_
#define MAX_INDEP_H_
#include "./td.h"
#include <vector>

/*
 * Adjacent matrix representation
 */
struct Graph {
  std::vector<std::vector<int> > adj; // n * n matrix
  std::vector<int> weight; // weights of vertices
};


int max_indep(const TreeDecomp &td, const Graph &g);
#endif // #ifndef MAX_INDEP_H_
