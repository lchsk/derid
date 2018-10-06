#ifndef CURSES_HPP
#define CURSES_HPP

#include <ncurses.h>
#include <string>

#include "label.hpp"
#include "list.hpp"
#include "pos.hpp"

namespace derid {
class curses {
public:
  curses();
  ~curses();

  derid::curses &set_col(const int col);
  derid::curses &set_row(const int row);
  derid::curses &set_pos(const int row, const int col);
  derid::curses &move();
  derid::curses &next_line();

  derid::curses &print(const derid::widget::label &);
  derid::curses &print(const derid::widget::list &);
  derid::curses &print(const std::string &);

  void reset();

  // TODO: Move this kind of logic out of here
  void update_label();

  void run();

  derid::widget::list *l;
  derid::widget::label *label;

  derid::pos size;

private:
  void clean(int row);
  void clean(const derid::widget::list &);

  template <typename F>
  bool execute_on_selected_entry(int list_index, int index, F f) const;

  template <typename Condition, typename Executor>
  void execute_on_condition(Condition, Executor) const;

  bool colors_available;
  derid::pos pos;
};
} // namespace derid

#endif
