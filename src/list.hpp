#ifndef LIST_HPP
#define LIST_HPP

#include "buffer.hpp"
#include "pos.hpp"

namespace derid {
namespace widget {
class list {
public:
  list(const derid::pos &p, const int items_shown);

  // OUT
  void refresh(const std::string &dir);

  bool next();

  bool prev();

  // OUT OF HERE
  bool enter();

  // OUT OF HERE
  bool jump_back();

  derid::buffer b;

  derid::pos pos;

  int index;
  int start;

  const int items_shown;
  const int half_items_shown;
};
} // namespace widget
} // namespace derid
#endif
