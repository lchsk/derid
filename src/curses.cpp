#include "curses.hpp"

namespace derid {

extern "C" void SignalHandler(int signum);

void Curses::InitTheme() {
    int color_id = 16;
    int color_pair_id = 16;

    for (const ColorPair &color_pair : color_theme_.GetColors()) {
        init_color(color_id, color_pair.fg_.r, color_pair.fg_.g,
                   color_pair.fg_.b);
        color_id++;
        init_color(color_id, color_pair.bg_.r, color_pair.bg_.g,
                   color_pair.bg_.b);
        color_id++;

        init_pair(color_pair_id, color_id - 2, color_id - 1);
        color_pairs_[color_pair.name_] = color_pair_id;

        color_pair_id++;
    }

    wbkgd(stdscr, DERID_COLOR("background"));
}

Curses::Curses(const ColorTheme &color_theme) : color_theme_(color_theme) {
    InitTerminal();

    ReadTermSize();

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
    const auto &pos = list.Position();

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
void Curses::ExecuteOnCondition(Condition condition, Executor executor) const {
    if (condition())
        executor();
}

Curses &Curses::Print(const widget::List &list) {
    pos_ = list.Position();
    Clean(list);

    move(pos_.row, pos_.col);

    const int max_index =
        std::min(list.Start() + list.ItemsShown(),
                 static_cast<int>(list.GetBuffer().Entries().size()));

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

        selected = ExecuteOnSelectedEntry(
            list.Index(), index, [&] { attroff(DERID_COLOR("selected")); });
        NextLine().Move();
    }

    return *this;
}

void Curses::Reset() {
    pos_.row = 0;
    pos_.col = 0;
    Move();
}

void Curses::UpdateLabel() {
    label_->SetText(list_->GetBuffer().Current().string());
}

void Curses::Run() {
    UpdateLabel();
    Print(*list_);
    Print(*label_);

    while (true) {
        input_.Read();
        const auto action = input_.GetAction();

        if (action == Input::InputAction::Quit) {
            break;
        }

        else if (action == Input::InputAction::Next) {
            if (list_->Next())
                Print(*list_);
        }

        else if (action == Input::InputAction::Prev) {
            if (list_->Prev())
                Print(*list_);
        } else if (action == Input::InputAction::Refresh) {
            list_->Refresh(list_->GetBuffer().GetAbsolute(
                list_->GetBuffer().GetCurrentPath()));
            Print(*list_);
        } else if (action == Input::InputAction::Enter) {
            if (list_->Enter()) {
                UpdateLabel();
                Print(*list_);
                Print(*label_);
            }
        }

        else if (action == Input::InputAction::Jump) {
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

const Pos &Curses::Size() const { return size_; }

void Curses::SetList(widget::List *list) { list_ = list; }
void Curses::SetLabel(widget::Label *label) { label_ = label; }

void Curses::InitTerminal() {
    initscr();

    // No echoing. If you have called noecho(), the character ch will not be
    // printed on the screen, otherwise it will.
    // Disabling automatic echoing gives you more control over the user
    // interface.
    noecho();

    // No buffering. If you have called cbreak(void) each key the user hits is
    // returned immediately by getch().
    // Otherwise the keys hit by the user are queued until a newline is read.
    // Then calls to getch() take characters from the queue in FIFO manner
    // until the queue is empty and the next whole line is read.
    cbreak();

    // The cbreak() function sets the input mode for the current terminal
    // to cbreak mode and overrides a call to raw().

    // The nocbreak() function sets the input mode for the current terminal to
    // Cooked Mode without changing the state of ISIG and IXON.

    // The noraw() function sets the input mode for the current
    // terminal to Cooked Mode and sets the ISIG and IXON flags.

    // The raw() function sets the input mode for the current
    // terminal to Raw Mode.
    // raw();

    // Special keys. If you have called keypad(stdstr, TRUE), then if the user
    // hits a special key such as the Delete key, the arrow keys, Ctrl
    // combined keys and function keys, a single int value will be returned.
    // Here is the definition of several special keys
    // key code        description

    // KEY_DOWN        The four arrow keys ...
    // KEY_UP
    // KEY_LEFT
    // KEY_RIGHT
    // KEY_HOME        Home key
    // KEY_BACKSPACE   Backspace
    // KEY_F(n)        Function keys, for 0 <= n >= 63
    // KEY_DC          Delete character
    // KEY_IC          Insert char or enter insert mode
    // KEY_ENTER       Enter or send
    // keypad(stdscr, TRUE);

    // control blocking on input
    // delay < 0
    // One or more blocking reads (indefinite waits for input) are used.
    // delay = 0
    // One or more non-blocking reads are used. Any Curses input function
    // will fail if every character of the requested string is not
    // immediately available.
    // delay > 0
    // Any Curses input function blocks for delay milliseconds
    // and fails if there is still no input.
    timeout(0);

    // The nodelay() function specifies whether Delay Mode or No Delay Mode
    // is in effect for the screen associated with the specified window.
    // If bf is TRUE, this screen is set to No Delay Mode.
    // If bf is FALSE, this screen is set to Delay Mode.
    // The initial state is FALSE.
    // nodelay(stdscr, true);

    // The notimeout() function controls whether or not getch() will wait
    // indefinitely between characters in a multi-character key sequence or not.
    // If flag is TRUE, then there is no timeout applied between characters
    // comprising a multi-character key sequence.  If flag is FALSE, then the
    // component characters of a multi-character sequence must not have an
    // inter-character gap of more than ESCDELAY.  If this timing is exceeded,
    // then the multi-character key assembly is deemed to have failed and the
    // characters read thus far are returned one at a time when getch() is
    // called.  The default setting for the flag is FALSE.  The default value of
    // ESCDELAY is 300ms.  If ESCDELAY is negative, no timeout is applied
    // between characters comprising a multi-character key sequence.
    // set_escdelay(0);

    // The curs_set() function sets the appearance of the cursor
    // based on the value of visibility:
    // 0 	Invisible
    // 1 	Terminal-specific normal mode
    // 2 	Terminal-specific high visibility mode
    curs_set(0);

    signal(SIGWINCH, SignalHandler);
}

void Input::Read() {
    action_ = Input::InputAction::None;

    const char in = getch();

    if (in == 'q') {
        action_ = Input::InputAction::Quit;
    } else if (in == Input::K_ESC_OR_ALT) {
        const char in_2 = getch();

        if (in_2 == '[') {
            // Arrow keys
            const char in_3 = getch();

            if (in_3 == Input::K_ARROW_UP) {
                action_ = Input::InputAction::Prev;
            } else if (in_3 == Input::K_ARROW_DOWN) {
                action_ = Input::InputAction::Next;
            }
        }

        if (in_2 == -1) {
            action_ = Input::InputAction::Quit;
        }
    }

    else if (in == 'n') {
        action_ = Input::InputAction::Next;
    } else if (in == 'p') {
        action_ = Input::InputAction::Prev;
    } else if (in == 'g') {
        action_ = Input::InputAction::Refresh;
    } else if (in == 'e' || in == Input::K_ENTER) {
        action_ = Input::InputAction::Enter;
    } else if (in == 'j') {
        action_ = Input::InputAction::Jump;
    }
}

void Curses::ReadTermSize() {
    int row, col;

    getmaxyx(stdscr, row, col);

    size_.row = row;
    size_.col = col;
}

void Curses::ResizeTermHandler() {
    ReadTermSize();

    refresh();
}

const Input::InputAction Input::GetAction() const { return action_; }

extern "C" void SignalHandler(int signum) {
    if (Curses::signal_object == nullptr) {
        return;
    }

    if (signum == SIGWINCH) {
        Curses::signal_object->ResizeTermHandler();
    }
}

Curses *Curses::signal_object = nullptr;

} // namespace derid
