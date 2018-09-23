#define LOGURU_IMPLEMENTATION 1

#include "buffer.hpp"
#include "curses.hpp"
#include "label.hpp"
#include "list.hpp"
#include "pos.hpp"

int main(int argc, char *argv[]) {
    boost::log::add_file_log("sample.log");

    derid::curses c;

    // 4 empty lines under the list
    derid::widget::list l(derid::pos(1, 0), c.size.row - 5);
    derid::widget::label label(derid::pos(0, 0), "");

    std::vector<std::string> t;

    for (int i = 0; i < 20; i++) {
        std::string s;
        s += std::to_string(i + 1) + ") ";

        for (int j = 0; j < rand() % 10 + 2; j++) {

            s += "a";
        }
        t.push_back(s);
    }

    // l.items = t;
    // l.items = b.list;
    c.l = &l;
    c.label = &label;

    // c.print(l);
    // c.print(l);

    c.run();
    // getch();

    // c.~curses();
}
