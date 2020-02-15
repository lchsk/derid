#include "buffer.hpp"
#include "curses.hpp"
#include "widgets/label.hpp"
#include "widgets/list.hpp"
#include "pos.hpp"
#include "colors.hpp"
#include "config.hpp"

using namespace derid;

int main(int argc, char *argv[]) {
    Config config;
    config.Read();

    ColorTheme color_theme;
    color_theme.ReadFromConfig(config);

    Curses curses(color_theme);

    // 4 empty lines under the list
    widget::List list(Pos(1, 0), curses.Size().row - 5);
    widget::Label label(Pos(0, 0), "");
    label.SetColor(COLOR_PAIR(curses.color_pairs_["current_path"]));
    // widget::input input(pos(0, curses.size.get_row() - 3));

    curses.SetList(&list);
    curses.SetLabel(&label);
    // curses.input = &input;

    curses.Run();
}
