#ifndef CURSES_HPP
#define CURSES_HPP

#include <ncurses.h>
#include <string>
#include <unordered_map>

#include "widgets/label.hpp"
#include "widgets/list.hpp"
#include "pos.hpp"
#include "colors.hpp"

#define DERID_COLOR(color_name) COLOR_PAIR(color_pairs_[color_name])

namespace derid {
class curses {
  public:
    curses(const ColorTheme& color_theme);
    ~curses();



    curses &set_col(const int col);
    curses &set_row(const int row);
    curses &set_pos(const int row, const int col);
    curses &move();
    curses &next_line();

    curses &print(const widget::Label &);
    curses &print(const widget::List &);
    curses &print(const std::string &);

    void reset();

    // TODO: Move this kind of logic out of here
    void update_label();

    void run();

    derid::widget::List *l;
    derid::widget::Label *label;
    // derid::widget::input *input;

    derid::Pos size;

    // TODO: Hide this
    std::unordered_map<std::string, int> color_pairs_;

  private:
    void clean(int row);
    void clean(const widget::List &);

    void InitTheme();

    template <typename F>
    bool execute_on_selected_entry(int list_index, int index, F f) const;

    template <typename Condition, typename Executor>
    void execute_on_condition(Condition, Executor) const;

    bool colors_available;
    derid::Pos pos;
    const ColorTheme& color_theme_;
};
} // namespace derid

#endif
