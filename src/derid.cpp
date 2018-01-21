#include <ncurses.h>

namespace derid
{
    class curses
    {
    public:
        curses() : colors_available(false)
        {
            initscr();
            raw();
            noecho();
            keypad(stdscr, 1);

            colors_available = has_colors();

            if (colors_available)
                start_color();
        }

        ~curses()
        {
            if (! isendwin()) {
                endwin();
            }
        }

    private:
        bool colors_available;
    };
}

int main()
{
    derid::curses curses;
}
