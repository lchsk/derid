#ifndef CURSES_HPP
#define CURSES_HPP

#include <ncurses.h>
#include <string>
#include <unordered_map>

#include "colors.hpp"
#include "pos.hpp"
#include "widgets/label.hpp"
#include "widgets/list.hpp"

// Change it to a function
#define DERID_COLOR(color_name) COLOR_PAIR(color_pairs_[color_name])

namespace derid {
class Input {
  public:
    enum class InputAction {
        None,
        Quit,
        Next,
        Prev,
        Refresh,
        Enter,
        Jump,
    };

    void Read();
    const InputAction GetAction() const;

  private:
    InputAction action_;

    const static int KEY_ESC_OR_ALT = 27;
};

class Curses {
  public:
    Curses(const ColorTheme &color_theme);
    ~Curses();

    Curses &SetCol(const int col);
    Curses &SetRow(const int row);
    Curses &SetPos(const int row, const int col);
    Curses &Move();
    Curses &NextLine();

    Curses &Print(const widget::Label &);
    Curses &Print(const widget::List &);
    Curses &Print(const std::string &);

    void Reset();

    // TODO: Move this kind of logic out of here
    void UpdateLabel();

    void Run();

    // TODO: Hide this
    std::unordered_map<std::string, int> color_pairs_;

    const Pos &Size() const;

    void SetList(widget::List *list);
    void SetLabel(widget::Label *label);

  private:
    void InitTerminal();
    void Clean(int row);
    void Clean(const widget::List &);

    void InitTheme();

    template <typename F>
    bool ExecuteOnSelectedEntry(int list_index, int index, F f) const;

    template <typename Condition, typename Executor>
    void ExecuteOnCondition(Condition, Executor) const;

    bool colors_available_;
    Pos pos_;
    Pos size_;

    const ColorTheme &color_theme_;

    Input input_;

    // TODO: This stuff should be in its own UI
    widget::List *list_;
    widget::Label *label_;
    // derid::widget::input *input;
};
} // namespace derid

#endif
