#ifndef POS_HPP
#define POS_HPP

namespace derid {
struct Pos {
    Pos(int row, int col);
    Pos();

    int row, col;
};

} // namespace derid

#endif
