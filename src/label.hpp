#ifndef LABEL_HPP
#define LABEL_HPP

#include <ncurses.h>

#include "pos.hpp"

namespace derid {
namespace widget {
class label {
  public:
    label() {}
    label(const derid::pos& p, const std::string& text)
        : pos(p), text(text), color(COLOR_PAIR(5))
    {
    }

    derid::pos pos;
    std::string text;
    int color;
};
} // namespace widget
} // namespace derid
#endif
