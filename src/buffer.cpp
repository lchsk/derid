#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/regex.hpp>

#include "buffer.hpp"

namespace derid {
const std::string ExecShellCmd(const std::string &cmd) {
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

std::vector<std::string> SplitOnNewLine(const std::string &text) {
    std::stringstream ss(text);
    std::string line, temp;
    std::vector<std::string> lines;

    while (std::getline(ss, temp, '\n')) {
        lines.push_back(temp);
    }

    return lines;
}

bool Buffer::IsFile(const fs::path &p) {
    return fs::exists(p) && fs::is_regular_file(p);
}

const fs::path Buffer::GetCurrentPath() const {
    return boost::filesystem::current_path();
}

std::string Buffer::GetAbsolute(const fs::path &p) const {
    return fs::absolute(p).string();
}

std::string Buffer::GetAbsolute(int index) {
    return fs::absolute(paths_[index]).string();
}

// const std::string Buffer::GetLine(int index) const {
// const auto &entry = entries_[index];

// std::string line = format;

// boost::replace_all(line, "%name", entry.FmtName());
// boost::replace_all(line, "%perms", entry.Perms());
// boost::replace_all(line, "%owner", entry.FmtOwner());
// boost::replace_all(line, "%group", entry.FmtGroup());
// boost::replace_all(line, "%size", entry.FmtSize());
// boost::replace_all(line, "%year", entry.year);
// boost::replace_all(line, "%month", entry.month);
// boost::replace_all(line, "%day", entry.fmt_day);
// boost::replace_all(line, "%datetime", entry.FmtDatetime());
// boost::replace_all(line, "%time", entry.time);

// return line;
// }

const std::vector<std::pair<std::string, std::string>>
Buffer::GetLineData(int index) const {
    std::vector<std::string> parts;
    parts.reserve(8);
    boost::split(parts, format, boost::is_any_of(" "));

    const auto &entry = entries_[index];

    std::vector<std::pair<std::string, std::string>> line_map;
    std::unordered_map<std::string, std::string> mapping{
        {"%name", entry.FmtName()},
        {"%perms", entry.Perms()},
        {"%owner", entry.FmtOwner()},
        {"%group", entry.FmtGroup()},
        {"%size", entry.FmtSize()},
        {"%datetime", entry.GetFormattedDatetime(datetime_format)},
        {"%space", " "},
    };

    for (const std::string &part : parts) {
        if (part == "") {
            line_map.push_back(std::make_pair("%space", mapping["%space"]));
            continue;
        }

        line_map.push_back(std::make_pair(part, mapping[part]));

        line_map.push_back(std::make_pair("%space", mapping["%space"]));
    }

    return line_map;
}

const std::string Buffer::GetEntryByIndex(int index) {
    return entries_[index].Name();
}

void Buffer::ReadDir(const std::string &dir) {
    std::stringstream ss;
    // TODO: apostr
    ss << "ls -lh -D -a --quoting-style=literal --time-style=long-iso "
       << "'" << dir << "'";

    // list_.clear();
    paths_.clear();
    entries_.clear();

    current_ = dir;
    current_ = fs::canonical(current_);

    const std::string s = ExecShellCmd(ss.str());

    std::vector<std::string> all_lines = SplitOnNewLine(s);

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

        BufferEntry entry(name, parts);
        entries_.push_back(entry);

        if (entry.Size().size() > size_max) {
            size_max = entry.Size().size();
        }

        if (entry.Name().size() > name_max) {
            name_max = entry.Name().size();
        }

        if (entry.Owner().size() > owner_max) {
            owner_max = entry.Owner().size();
        }

        if (entry.Group().size() > group_max) {
            group_max = entry.Group().size();
        }
    }

    for (auto &entry : entries_) {
        std::stringstream ss_size;

        ss_size << std::setw(size_max) << entry.Size();
        entry.SetFmtSize(ss_size.str());

        std::stringstream ss_name;

        ss_name << std::left << std::setw(name_max) << entry.Name();
        entry.SetFmtName(ss_name.str());

        std::stringstream ss_owner;

        ss_owner << std::left << std::setw(owner_max) << entry.Owner();
        entry.SetFmtOwner(ss_owner.str());

        std::stringstream ss_group;

        ss_group << std::left << std::setw(group_max) << entry.Group();
        entry.SetFmtGroup(ss_group.str());
    }
}
const fs::path &Buffer::Current() const { return current_; }

const std::vector<BufferEntry> &Buffer::Entries() const { return entries_; }

} // namespace derid
