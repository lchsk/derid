#ifndef BUFFER_HPP
#define BUFFER_HPP
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>

#include "buffer_entry.hpp"

namespace derid {
namespace fs = boost::filesystem;

const std::string exec_shell_cmd(const std::string &);
std::vector<std::string> split_on_new_line(const std::string &);

class buffer {
  public:
    void read_dir(const std::string &dir);

    bool is_file(const fs::path &p);

    const fs::path get_current_path() const;

    std::string get_absolute(const fs::path &p) const;
    std::string get_absolute(int index);

    const std::string get_entry_by_index(int index);

    std::string dir;
    fs::path current;

    std::vector<std::string> list;
    std::vector<fs::path> paths;
    std::vector<BufferEntry> entries;

    const std::vector<std::pair<std::string, std::string>>
    get_line_data(int index) const;

  private:
    const std::string get_line(int index) const;
    const std::string format = "%perms %owner %group %size %datetime %name";

    const std::string datetime_format = "%Y-%m-%d %H:%M";
};
} // namespace derid

#endif
