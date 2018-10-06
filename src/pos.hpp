#ifndef POS_HPP
#define POS_HPP

namespace derid {
class pos {
public:
  pos(const int row, const int col) : row(row), col(col) {}
  pos() : pos(0, 0) {}

  int row, col;
};

} // namespace derid

#endif
