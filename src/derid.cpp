#include "buffer.hpp"
#include "curses.hpp"
#include "widgets/label.hpp"
#include "list.hpp"
#include "pos.hpp"
#include "colors.hpp"

using namespace derid;

int main(int argc, char *argv[]) {
    ColorTheme color_theme;
    color_theme.selected = ColorPair("selected", "ffffff", "3f88c5");
    color_theme.executable = ColorPair("executable", "e3655b", "000000");
    color_theme.directory = ColorPair("directory", "ffba08", "000000");
    color_theme.background = ColorPair("background", "ffffff", "000000");
    color_theme.current_path = ColorPair("current_path", "cbff8c", "000000");

    derid::curses curses(color_theme);

    // 4 empty lines under the list
    widget::list list(Pos(1, 0), curses.size.row - 5);
    widget::Label label(Pos(0, 0), "");
    label.SetColor(COLOR_PAIR(curses.color_pairs_["current_path"]));
    // widget::input input(pos(0, curses.size.get_row() - 3));

    curses.l = &list;
    curses.label = &label;
    // curses.input = &input;

    curses.run();
}
