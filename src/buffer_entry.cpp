#include "buffer_entry.hpp"

namespace derid {


buffer_entry::buffer_entry(const std::string &name,
                           const std::vector<std::string> &parts)
    : name(name) {
    if (parts.size() < min_parts_sz) {
        return;
    }

    perms = parts[to_type<entry_type>(entry_type::perms)];
    owner = parts[to_type<entry_type>(entry_type::owner)];
    group = parts[to_type<entry_type>(entry_type::group)];
    size = parts[to_type<entry_type>(entry_type::size)];
    month = parts[to_type<entry_type>(entry_type::month)];
    day = parts[to_type<entry_type>(entry_type::day)];
    time = parts[to_type<entry_type>(entry_type::time)];
}


}
