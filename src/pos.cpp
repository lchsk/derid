#include "pos.hpp"

namespace derid {
pos::pos(int row, int col) : m_row(row), m_col(col) {}
pos::pos() : pos(0, 0) {}
} // namespace derid
