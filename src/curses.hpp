#ifndef CURSES_HPP
#define CURSES_HPP

#include <ncurses.h>
#include <string>
#include <unordered_map>

#include "label.hpp"
#include "list.hpp"
#include "pos.hpp"
#include "colors.hpp"

#define DERID_COLOR(color_name) COLOR_PAIR(color_pairs_[color_name])

namespace derid {
class curses {
  public:
    curses(const ColorTheme& color_theme);
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
    derid::widget::input *input;

    derid::pos size;

    // TODO: Hide this
    std::unordered_map<std::string, int> color_pairs_;

  private:
    void clean(int row);
    void clean(const derid::widget::list &);

    void InitTheme();

    template <typename F>
    bool execute_on_selected_entry(int list_index, int index, F f) const;

    template <typename Condition, typename Executor>
    void execute_on_condition(Condition, Executor) const;

    bool colors_available;
    derid::pos pos;
    const ColorTheme& color_theme_;
};
} // namespace derid

#endif
