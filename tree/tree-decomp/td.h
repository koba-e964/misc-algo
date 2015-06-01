#ifndef TD_H_
#define TD_H_
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



struct NiceTreeDecomp {
  enum NodeType {
    Leaf,
    Intro,
    Forget,
    Join
  };
  std::vector<int> parent;
  std::vector<std::vector<int> > children;
  std::vector<NodeType> type;
  std::vector<int> prop;
  std::vector<std::vector<int> > bag;
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
std::istream &operator>>(std::istream &is, NiceTreeDecomp &td);
std::ostream &operator<<(std::ostream &os, const NiceTreeDecomp &td);

#endif // #ifndef TD_H_
