#include "curses.hpp"


namespace derid {
    void curses::InitTheme() {
        int color_id = 9;
        int color_pair_id = 1;

        for (const ColorPair& color_pair: color_theme_.GetColors()) {
            init_color(color_id, color_pair.fg_.r, color_pair.fg_.g, color_pair.fg_.b);
            color_id++;
            init_color(color_id, color_pair.bg_.r, color_pair.bg_.g, color_pair.bg_.b);
            color_id++;

            init_pair(color_pair_id, color_id - 2, color_id - 1);
            color_pairs_[color_pair.name_] = color_pair_id;

            color_pair_id++;
        }

        wbkgd(stdscr, DERID_COLOR("background"));
    }

    curses::curses(const ColorTheme& color_theme) :
        color_theme_(color_theme)
    {
    initscr();
    raw();
    noecho();
    keypad(stdscr, 1);

    curs_set(0);

    int row, col;

    getmaxyx(stdscr, row, col);
    size.row = row;
    size.col = col;

    colors_available = has_colors();

    if (colors_available) {
        start_color();

        InitTheme();

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
    pos.col = col;
    return *this;
}
derid::curses &curses::set_row(const int row) {
    pos.row = row;
    return *this;
}
derid::curses &curses::set_pos(const int row, const int col) {
    set_col(col);
    set_row(row);

    return *this;
}
derid::curses &curses::move() {
    ::move(pos.row, pos.col);

    return *this;
}

derid::curses &curses::next_line() {
    pos.row++;

    return *this;
}

curses &curses::print(const widget::Label &label) {
    clean(label.Position().row);

    ::move(label.Position().row, label.Position().col);

    if (label.Color() != -1) {
        attron(label.Color());
    }
    print(label.Text());
    if (label.Color() != -1) {
        attroff(label.Color());
    }

    return *this;
}

void curses::clean(const widget::List &list) {
    const auto& pos = list.Position();

    for (int i = pos.row; i < pos.row + list.ItemsShown(); i++) {
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

derid::curses &curses::print(const widget::List &l) {
    pos = l.Position();
    clean(l);

    ::move(pos.row, pos.col);

    const int max_index =
        std::min(l.Start() + l.ItemsShown(), static_cast<int>(l.GetBuffer().Entries().size()));

    for (int i = l.Start(), index = 0; i < max_index; i++, index++) {
        bool selected = execute_on_selected_entry(
            l.Index(), index, [&] { attron(DERID_COLOR("selected")); });

        const auto line_map = l.GetBuffer().GetLineData(i);

        const auto &entry = l.GetBuffer().Entries()[i];

        for (const auto &info_type : line_map) {
            execute_on_condition(
                [&] {
                    return !selected and info_type.first == "%name" and
                        entry.Type() ==
                        BufferEntry::BufferEntryType::executable;
                }

                ,
                [&] { attron(DERID_COLOR("executable")); });
            execute_on_condition(
                [&] {
                    return !selected and info_type.first == "%name" and
                        entry.Type() ==
                        BufferEntry::BufferEntryType::directory;
                }

                ,
                [&] { attron(DERID_COLOR("directory")); });

            print(info_type.second);

            execute_on_condition(
                [&] {
                    return !selected and info_type.first == "%name" and
                        entry.Type() ==
                        BufferEntry::BufferEntryType::executable;
                }

                ,
                [&] { attroff(DERID_COLOR("executable")); });
            execute_on_condition(
                [&] {
                    return !selected and info_type.first == "%name" and
                        entry.Type() ==
                        BufferEntry::BufferEntryType::directory;
                }

                ,
                [&] { attroff(DERID_COLOR("directory")); });
        }

        selected = execute_on_selected_entry(l.Index(), index,
                                             [&] { attroff(DERID_COLOR("selected")); });
        next_line().move();
    }

    return *this;
}

void curses::reset() {
    pos.row = 0;
    pos.col = 0;
    move();
}

    void curses::update_label() { label->SetText(l->GetBuffer().Current().string()); }

void curses::run() {
    update_label();
    print(*l);
    print(*label);

    while (true) {
        const char in = getch();

        if (in == 'q') {
            break;
        } else if (in == 'n') {
            if (l->Next())
                print(*l);
        } else if (in == 'p') {
            if (l->Prev())
                print(*l);
        } else if (in == 'g') {
            l->Refresh(l->GetBuffer().GetAbsolute(l->GetBuffer().GetCurrentPath()));
            print(*l);
        } else if (in == '!') {

        } else if (in == 'e') {
            if (l->Enter()) {
                update_label();
                print(*l);
                print(*label);
            }

        } else if (in == 'j') {
            if (l->JumpBack()) {
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
