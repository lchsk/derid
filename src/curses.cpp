#include "curses.hpp"


namespace derid {
    void Curses::InitTheme() {
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

    Curses::Curses(const ColorTheme& color_theme) :
        color_theme_(color_theme)
    {
    initscr();
    raw();
    noecho();
    keypad(stdscr, 1);

    curs_set(0);

    int row, col;

    getmaxyx(stdscr, row, col);
    size_.row = row;
    size_.col = col;

    colors_available_ = has_colors();

    if (colors_available_) {
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

Curses::~Curses() {
    if (!isendwin()) {
        endwin();
    }
}

Curses &Curses::Print(const std::string &str) {
    for (char ch : str) {
        addch(ch);
    }

    return *this;
}

Curses &Curses::SetCol(const int col) {
    pos_.col = col;
    return *this;
}
Curses &Curses::SetRow(const int row) {
    pos_.row = row;
    return *this;
}
Curses &Curses::SetPos(const int row, const int col) {
    SetCol(col);
    SetRow(row);

    return *this;
}
Curses &Curses::Move() {
    move(pos_.row, pos_.col);

    return *this;
}

Curses &Curses::NextLine() {
    pos_.row++;

    return *this;
}

Curses &Curses::Print(const widget::Label &label) {
    Clean(label.Position().row);

    move(label.Position().row, label.Position().col);

    if (label.Color() != -1) {
        attron(label.Color());
    }
    Print(label.Text());
    if (label.Color() != -1) {
        attroff(label.Color());
    }

    return *this;
}

void Curses::Clean(const widget::List &list) {
    const auto& pos = list.Position();

    for (int i = pos.row; i < pos.row + list.ItemsShown(); i++) {
        Clean(i);
    }
}

template <typename F>
bool Curses::ExecuteOnSelectedEntry(int list_index, int index, F f) const {
    if (list_index == index) {
        f();
        return true;
    }

    return false;
}

template <typename Condition, typename Executor>
void Curses::ExecuteOnCondition(Condition condition,
                                  Executor executor) const {
    if (condition())
        executor();
}

Curses &Curses::Print(const widget::List &list) {
    pos_ = list.Position();
    Clean(list);

    move(pos_.row, pos_.col);

    const int max_index =
        std::min(list.Start() + list.ItemsShown(), static_cast<int>(list.GetBuffer().Entries().size()));

    for (int i = list.Start(), index = 0; i < max_index; i++, index++) {
        bool selected = ExecuteOnSelectedEntry(
            list.Index(), index, [&] { attron(DERID_COLOR("selected")); });

        const auto line_map = list.GetBuffer().GetLineData(i);

        const auto &entry = list.GetBuffer().Entries()[i];

        for (const auto &info_type : line_map) {
            ExecuteOnCondition(
                [&] {
                    return !selected and info_type.first == "%name" and
                        entry.Type() ==
                        BufferEntry::BufferEntryType::executable;
                }

                ,
                [&] { attron(DERID_COLOR("executable")); });
            ExecuteOnCondition(
                [&] {
                    return !selected and info_type.first == "%name" and
                        entry.Type() ==
                        BufferEntry::BufferEntryType::directory;
                }

                ,
                [&] { attron(DERID_COLOR("directory")); });

            Print(info_type.second);

            ExecuteOnCondition(
                [&] {
                    return !selected and info_type.first == "%name" and
                        entry.Type() ==
                        BufferEntry::BufferEntryType::executable;
                }

                ,
                [&] { attroff(DERID_COLOR("executable")); });
            ExecuteOnCondition(
                [&] {
                    return !selected and info_type.first == "%name" and
                        entry.Type() ==
                        BufferEntry::BufferEntryType::directory;
                }

                ,
                [&] { attroff(DERID_COLOR("directory")); });
        }

        selected = ExecuteOnSelectedEntry(list.Index(), index,
                                             [&] { attroff(DERID_COLOR("selected")); });
        NextLine().Move();
    }

    return *this;
}

void Curses::Reset() {
    pos_.row = 0;
    pos_.col = 0;
    Move();
}

    void Curses::UpdateLabel() { label_->SetText(list_->GetBuffer().Current().string()); }

void Curses::Run() {
    UpdateLabel();
    Print(*list_);
    Print(*label_);

    while (true) {
        const char in = getch();

        if (in == 'q') {
            break;
        } else if (in == 'n') {
            if (list_->Next())
                Print(*list_);
        } else if (in == 'p') {
            if (list_->Prev())
                Print(*list_);
        } else if (in == 'g') {
            list_->Refresh(list_->GetBuffer().GetAbsolute(list_->GetBuffer().GetCurrentPath()));
            Print(*list_);
        } else if (in == '!') {

        } else if (in == 'e') {
            if (list_->Enter()) {
                UpdateLabel();
                Print(*list_);
                Print(*label_);
            }

        } else if (in == 'j') {
            if (list_->JumpBack()) {
                UpdateLabel();
                Print(*list_);
                Print(*label_);
            }
        }

        refresh();
    }
}

void Curses::Clean(int row) {
    move(row, 0);
    clrtoeol();
}

    const Pos& Curses::Size() const {
        return size_;
    }

    void Curses::SetList(widget::List *list) {
        list_ = list;
    }
    void Curses::SetLabel(widget::Label *label) {
        label_ = label;
    }


} // namespace derid
