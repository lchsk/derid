#include "curses.hpp"

namespace derid {
curses::curses() {
    initscr();
    raw();
    noecho();
    keypad(stdscr, 1);

    curs_set(0);

    getmaxyx(stdscr, size.row, size.col);

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

derid::curses &curses::print(const derid::widget::label &l) {
    clean_line(l.pos.row);

    ::move(l.pos.row, l.pos.col);

    attron(l.color);
    print(l.text);
    attroff(l.color);

    return *this;
}

derid::curses &curses::print(const derid::widget::list &l) {
    pos = l.pos;
    int index = 0;

    for (int i = pos.row; i < pos.row + l.items_shown; i++) {
        clean_line(i);
    }

    ::move(pos.row, pos.col);

    const int max_index = std::max(l.start + l.items_shown, static_cast<int>(l.b.entries.size()));

    for (int i = l.start; i < max_index; i++) {
        if (l.index == index) {
            attron(COLOR_PAIR(1));
        }

        print(l.b.get_line(i));

        if (l.index == index) {
            attroff(COLOR_PAIR(1));
        }

        next_line().move();

        index++;
    }

    return *this;
}

void curses::reset() {
    pos.row = 0;
    pos.col = 0;
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

void curses::clean_line(int row) {
    ::move(row, 0);
    clrtoeol();
}

} // namespace derid
