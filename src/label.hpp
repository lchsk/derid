#ifndef LABEL_HPP
#define LABEL_HPP

#include <ncurses.h>

#include "pos.hpp"

namespace derid {
namespace widget {
class label {
  public:
    label() {}
    label(const derid::pos &p, const std::string &text)
        : pos(p), text(text), color_(-1) {}

    derid::pos pos;
    std::string text;
    int color_;
};

    class input {
    public:
        input(const derid::pos&p) : pos(p) {}


        derid::pos pos;
        std::string text;
    };

} // namespace widget
} // namespace derid
#endif
