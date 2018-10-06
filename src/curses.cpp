#include "curses.hpp"

namespace derid {
curses::curses() {
  initscr();
  raw();
  noecho();
  keypad(stdscr, 1);

  curs_set(0);

  int row, col;

  getmaxyx(stdscr, row, col);
  size.set_row(row);
  size.set_col(col);

  colors_available = has_colors();

  if (colors_available) {
    start_color();

    for (int f = 0; f < COLORS; f++) {
      for (int b = 0; b < COLORS; b++) {
        init_pair(f * COLORS + b, f, b);
      }
    }

    if (can_change_color()) {
      // init_color(XS_COL_SEL, col_sel.r, col_sel.g, col_sel.b);
      // init_pair(1, COLOR_WHITE, XS_COL_SEL);
      // init_pair(1, COLOR_WHITE, COLOR_BLUE);
    } else {
      // init_pair(1, COLOR_WHITE, COLOR_BLUE);
    }
  }
}

curses::~curses() {
  if (!isendwin()) {
    endwin();
  }
}

derid::curses &curses::print(const std::string &str) {
  for (char ch : str) {
    addch(ch);
  }

  return *this;
}

derid::curses &curses::set_col(const int col) {
  pos.set_col(col);
  return *this;
}
derid::curses &curses::set_row(const int row) {
  pos.set_row(row);
  return *this;
}
derid::curses &curses::set_pos(const int row, const int col) {
  set_col(col);
  set_row(row);

  return *this;
}
derid::curses &curses::move() {
  ::move(pos.get_row(), pos.get_col());

  return *this;
}

derid::curses &curses::next_line() {
  pos.set_row(pos.get_row() + 1);

  return *this;
}

derid::curses &curses::print(const derid::widget::label &label) {
  clean(label.pos.get_row());

  ::move(label.pos.get_row(), label.pos.get_col());

  attron(label.color);
  print(label.text);
  attroff(label.color);

  return *this;
}

void curses::clean(const derid::widget::list &list) {
  const auto pos = list.pos;

  for (int i = pos.get_row(); i < pos.get_row() + list.items_shown; i++) {
    clean(i);
  }
}

template <typename F>
bool curses::execute_on_selected_entry(int list_index, int index, F f) const {
  if (list_index == index) {
    f();
    return true;
  }

  return false;
}

template <typename Condition, typename Executor>
void curses::execute_on_condition(Condition condition,
                                  Executor executor) const {
  if (condition())
    executor();
}

derid::curses &curses::print(const derid::widget::list &l) {
  pos = l.pos;
  clean(l);

  ::move(l.pos.get_row(), l.pos.get_col());

  const int max_index =
      std::min(l.start + l.items_shown, static_cast<int>(l.b.entries.size()));

  for (int i = l.start, index = 0; i < max_index; i++, index++) {
    bool selected = execute_on_selected_entry(l.index, index,
                                              [] { attron(COLOR_PAIR(1)); });

    const auto line_map = l.b.get_line_data(i);

    const auto &entry = l.b.entries[i];

    for (const auto &info_type : line_map) {
      execute_on_condition(
          [&] {
            return !selected and info_type.first == "%name" and
                   entry.m_object_type == buffer_entry::object_type::executable;
          }

          ,
          [] { attron(COLOR_PAIR(4)); });

      print(info_type.second);

      execute_on_condition(
          [&] {
            return !selected and info_type.first == "%name" and
                   entry.m_object_type == buffer_entry::object_type::executable;
          }

          ,
          [] { attroff(COLOR_PAIR(4)); });
    }

    selected = execute_on_selected_entry(l.index, index,
                                         [] { attroff(COLOR_PAIR(1)); });
    next_line().move();
  }

  return *this;
}

void curses::reset() {
  pos.set_row(0);
  pos.set_col(0);
  move();
}

void curses::update_label() { label->text = l->b.current.string(); }

void curses::run() {
  update_label();
  print(*l);
  print(*label);

  while (true) {
    const char in = getch();

    if (in == 'q') {
      break;
    } else if (in == 'n') {
      if (l->next())
        print(*l);
    } else if (in == 'p') {
      if (l->prev())
        print(*l);
    } else if (in == 'e') {
      if (l->enter()) {
        update_label();
        print(*l);
        print(*label);
      }

    } else if (in == 'j') {
      if (l->jump_back()) {
        update_label();
        print(*l);
        print(*label);
      }
    }

    refresh();
  }
}

void curses::clean(int row) {
  ::move(row, 0);
  clrtoeol();
}

} // namespace derid
