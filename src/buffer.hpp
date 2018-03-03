#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <boost/filesystem.hpp>
#define BOOST_LOG_DYN_LINK 1
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>

namespace derid {
namespace fs = boost::filesystem;

    class buffer_entry
    {
    public:
        buffer_entry(const fs::path& path) : path(path) {}

        const std::string get_filename() const {
            return path.filename().string();
        }

        const bool is_directory() const {
            return fs::is_directory(path);
        }

        const bool is_regular_file() const {
            return fs::is_regular_file(path);
        }

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

    std::string dir;
    fs::path current;

    std::vector<std::string> list;
    std::vector<fs::path> paths;
    std::vector<derid::buffer_entry> entries;
};
} // namespace derid

#endif
