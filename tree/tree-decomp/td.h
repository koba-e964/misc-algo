#include <vector>
#include <istream>
#include <ostream>

struct TreeDecomp {
  std::vector<int> parent;
  std::vector<std::vector<int> > children;
  std::vector<std::vector<int> > bags;
  int wei;
  inline int size() const {
    return parent.size();
  }
  inline int width() const {
    return wei - 1;
  }
};
std::istream &operator>>(std::istream &is, TreeDecomp &td);
std::ostream &operator<<(std::ostream &os, const TreeDecomp &td);
