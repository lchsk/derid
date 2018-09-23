#include <sstream>
#include <iomanip>
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/regex.hpp>

#include "buffer.hpp"

namespace derid {
const std::string exec_shell_cmd(const std::string &cmd) {
    const int buffer_size = 128;

    std::array<char, buffer_size> buffer;

    std::string result = "";

    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);

    if (!pipe) {
        return result;
    }

    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), buffer_size, pipe.get()) != nullptr)
            result += buffer.data();
    }

    return result;
}

std::vector<std::string> split_on_new_line(const std::string &text) {
    std::stringstream ss(text);
    std::string line, temp;
    std::vector<std::string> lines;

    while (std::getline(ss, temp, '\n')) {
        lines.push_back(temp);
    }

    return lines;
}

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

bool buffer::is_file(const fs::path &p) {
    return fs::exists(p) && fs::is_regular_file(p);
}

const fs::path buffer::get_current_path() {
    return boost::filesystem::current_path();
}

std::string buffer::get_absolute(const fs::path &p) {
    return fs::absolute(p).string();
}

std::string buffer::get_absolute(int index) {
    return fs::absolute(paths[index]).string();
}

const std::string buffer::get_line(int index) const
{
    const auto &entry = entries[index];

    std::string line = format;

    boost::replace_all(line, "%name", entry.fmt_name);
    boost::replace_all(line, "%perms", entry.perms);
    boost::replace_all(line, "%owner", entry.fmt_owner);
    boost::replace_all(line, "%group", entry.fmt_group);
    boost::replace_all(line, "%size", entry.fmt_size);
    boost::replace_all(line, "%month", entry.month);
    boost::replace_all(line, "%day", entry.fmt_day);
    boost::replace_all(line, "%time", entry.time);

    return line;
}

const std::string buffer::get_entry_by_index(int index) {
    return entries[index].name;
}

void buffer::read_dir(const std::string &dir) {
    std::stringstream ss;
    ss << "ls -lh -D -a --quoting-style=literal "
       << "'" << dir << "'";

    list.clear();
    paths.clear();
    entries.clear();

    current = dir;
    current = fs::canonical(current);

    const std::string s = derid::exec_shell_cmd(ss.str());

    std::vector<std::string> all_lines = derid::split_on_new_line(s);

    boost::regex rx_numbers("[0-9]+");
    std::vector<std::string> tokens;
    std::vector<std::string> lines;

    for (auto it = all_lines.cbegin() + 1; it != all_lines.cend(); it++) {
        if (boost::algorithm::starts_with(*it, "//DIRED//")) {
            boost::algorithm::find_all_regex(tokens, *it, rx_numbers);
        } else if (boost::algorithm::starts_with(*it, "//DIRED-OPTIONS//")) {
            // pass
        } else {
            lines.push_back(*it);
        }
    }

    std::vector<std::string> names;
    std::vector<std::string> stats_lines;

    for (std::size_t i = 0; i < tokens.size() - 1; i += 2) {
        int start = std::stoi(tokens[i]);
        int len = std::stoi(tokens[i + 1]) - std::stoi(tokens[i]);

        names.push_back(s.substr(start, len));
    }

    assert(lines.size() == names.size());

    std::size_t size_max = 0;
    std::size_t name_max = 0;
    std::size_t owner_max = 0;
    std::size_t group_max = 0;

    for (std::size_t i = 0; i < lines.size(); i++) {
        const std::string &name = names[i];
        std::string line = lines[i];

        std::vector<std::string> parts;

        boost::trim(line);
        boost::split(parts, line, boost::is_any_of(" "));

        auto removed =
            std::remove_if(parts.begin(), parts.end(),
                           [](const auto &part) { return part.empty(); });

        parts.erase(removed, parts.end());

        derid::buffer_entry entry(name, parts);
        entries.push_back(entry);

        if (entry.size.size() > size_max) {
            size_max = entry.size.size();
        }

        if (entry.name.size() > name_max) {
            name_max = entry.name.size();
        }
    }

    for (auto& entry : entries) {
        std::stringstream ss_size;

        ss_size << std::setw(size_max) << entry.size;
        entry.fmt_size = ss_size.str();

        std::stringstream ss_name;

        ss_name << std::left << std::setw(name_max) << entry.name;
        entry.fmt_name = ss_name.str();

        std::stringstream ss_owner;

        ss_owner << std::left << std::setw(owner_max) << entry.owner;
        entry.fmt_owner = ss_owner.str();

        std::stringstream ss_group;

        ss_group << std::left << std::setw(group_max) << entry.group;
        entry.fmt_group = ss_group.str();

        std::stringstream ss_day;

        ss_day << std::right << std::setw(2) << entry.day;
        entry.fmt_day = ss_day.str();
    }
}

} // namespace derid
