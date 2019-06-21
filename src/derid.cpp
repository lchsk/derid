#include "buffer.hpp"
#include "curses.hpp"
#include "label.hpp"
#include "list.hpp"
#include "pos.hpp"

int main(int argc, char *argv[]) {
    derid::curses curses;

    // 4 empty lines under the list
    derid::widget::list list(derid::pos(1, 0), curses.size.get_row() - 5);
    derid::widget::label label(derid::pos(0, 0), "");

    curses.l = &list;
    curses.label = &label;

    curses.run();
}
