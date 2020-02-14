#include "pos.hpp"

namespace derid {
Pos::Pos(int row, int col) : row(row), col(col) {}
Pos::Pos() : Pos(0, 0) {}
} // namespace derid
