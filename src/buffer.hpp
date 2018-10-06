#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <boost/filesystem.hpp>
#define BOOST_LOG_DYN_LINK 1
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>

#include "buffer_entry.hpp"

namespace derid {
namespace fs = boost::filesystem;

const std::string exec_shell_cmd(const std::string &);
std::vector<std::string> split_on_new_line(const std::string &);


class buffer {
  public:
    void read_dir(const std::string &dir);

    bool is_file(const fs::path &p);

    const fs::path get_current_path();

    std::string get_absolute(const fs::path &p);
    std::string get_absolute(int index);

    const std::string get_entry_by_index(int index);

    std::string dir;
    fs::path current;

    std::vector<std::string> list;
    std::vector<fs::path> paths;
    std::vector<derid::buffer_entry> entries;

    const std::string get_line(int index) const;
private:
    const std::string format = "%perms %owner %group %size %month %day %time %name";
};
} // namespace derid

#endif
