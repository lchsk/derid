#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <boost/filesystem.hpp>
#define BOOST_LOG_DYN_LINK 1
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>

namespace derid {
namespace fs = boost::filesystem;

    const std::string exec_shell_cmd(const std::string&);
    std::vector<std::string> split_on_new_line(const std::string&);

    class buffer_entry
    {
    public:
        buffer_entry(const fs::path& path) : path(path) {}
        buffer_entry(const std::string& raw_line, const std::string& name, const std::string& stats_line);

        const std::string get_filename() const {
            return path.filename().string();
        }

        const bool is_directory() const {
            return fs::is_directory(path);
        }

        const bool is_regular_file() const {
            return fs::is_regular_file(path);
        }

        // std::string get_perms() const {
            // const fs::file_status status = fs::status(path);
            // const fs::perms perms = status.permissions();

            // std::string p = "          ";

            // if (p & fs::perms::owner_exe) {
            // }

            // return p;
        // }

        // ls stuff
        //

        // Raw data, as returned by ls
        const std::string raw_line;

        // Just a name, as provided by ls
        const std::string name;

        // Part of the line without the name
        const std::string stats_line;

        fs::path path;
    };

class buffer {
  public:
    void read_dir(const std::string& dir);

    bool is_file(const fs::path& p);

    const fs::path get_current_path();

    std::string get_absolute(const fs::path& p);
    std::string get_absolute(int index);

    const fs::path get_path_by_index(int index);
    const std::string get_entry_by_index(int index);

    std::string dir;
    fs::path current;

    std::vector<std::string> list;
    std::vector<fs::path> paths;
    std::vector<derid::buffer_entry> entries;
};
} // namespace derid

#endif
