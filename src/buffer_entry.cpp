#include "buffer_entry.hpp"

#include <iostream>

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
    date = parts[to_type<entry_type>(entry_type::date)];
    // month = parts[to_type<entry_type>(entry_type::month)];
    // day = parts[to_type<entry_type>(entry_type::day)];
    time = parts[to_type<entry_type>(entry_type::time)];

    std::istringstream ss(date + " " + time);

    ss >> std::get_time(&datetime, "%Y-%m-%d %H:%M");

    if (!ss.fail()) {
        // std::cout << std::put_time(&datetime, "%c") << '\n';
    }

    if (perms.size() == 10) {
        switch (perms[0]) {
        case 'd':
            m_object_type = object_type::directory;
            break;
        case '-':
            if (perms[3] == 'x' or perms[6] == 'x' or perms[9] == 'x') {
                m_object_type = object_type::executable;
            } else {
                m_object_type = object_type::file;
            }
            break;
        }
    } else {
        m_object_type = object_type::undefined;
    }
}

const std::string
buffer_entry::get_formatted_datetime(const std::string &format) const {
    std::stringstream out;

    out << std::put_time(&datetime, format.c_str());

    return out.str();
}
} // namespace derid
