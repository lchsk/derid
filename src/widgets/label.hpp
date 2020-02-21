#ifndef LABEL_HPP
#define LABEL_HPP

#include <string>

#include <ncurses.h>

#include "../pos.hpp"

namespace derid {
namespace widget {
class Label {
  public:
    Label(const Pos &p, const std::string &text);

    const Pos &Position() const;
    const std::string &Text() const;
    void SetText(const std::string &);
    int Color() const;
    void SetColor(int);

  private:
    const Pos pos_;
    std::string text_;
    int color_;
};

// class input {
// public:
// input(const derid::pos&p) : pos(p) {}

// derid::pos pos;
// std::string text;
// };

} // namespace widget
} // namespace derid
#endif
