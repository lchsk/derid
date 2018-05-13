#include "include/loguru.hpp"

#include "list.hpp"


namespace derid {
namespace widget {

list::list(const derid::pos& p, const int items_shown)
    : pos(p), index(0), start(0), items_shown(items_shown),
      half_items_shown(items_shown / 2)

{
    refresh(b.get_absolute(b.get_current_path()));
}

void list::refresh(const std::string& dir)
{
    b.read_dir(dir);
    index = start = 0;
}

bool list::next()
{
    // Return true of list needs to be refreshed
    int size = b.entries.size();
    // int size = b.list.size();

    if (index + start >= size - 1)
        return false;

    // ???
    index < size - 1 ? index++ : size - 1;

    if (index == items_shown) {
        start += half_items_shown;

        int idx = 0;

        if (size - start >= items_shown) {
            idx = half_items_shown;
        } else {
            const int left_out = (size % items_shown);

            idx = (size - start) - left_out +
                  (left_out > half_items_shown ? half_items_shown : 0);
        }

        index = idx;
    }

    return true;
}

bool list::prev()
{
    if (index + start <= 0)
        return false;

    index--;

    if (index == -1 && start >= half_items_shown) {
        start -= half_items_shown;
        index = half_items_shown - 1;
    }

    return true;
}

bool list::enter()
{
    const auto p = b.get_entry_by_index(start + index);

    auto np = fs::path(b.current);

    np += "/" + p;

    if (b.is_file(np)) {
        return false;
    }

    const auto abs = b.get_absolute(np);

    refresh(abs);

    return true;
}

bool list::jump_back()
{
    refresh(b.get_absolute(b.current.parent_path()));

    return true;
}

} // namespace widget
} // namespace derid
