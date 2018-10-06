#ifndef BUFFER_ENTRY_HPP
#define BUFFER_ENTRY_HPP

#include <boost/filesystem.hpp>

namespace derid {
namespace fs = boost::filesystem;

class buffer_entry {
  enum class entry_type : unsigned int {
    perms = 0,
    owner = 2,
    group = 3,
    size = 4,
    month = 5,
    day = 6,
    time = 7,
  };

  const std::size_t min_parts_sz = 8;

  template <typename E> constexpr auto to_type(E e) {
    return static_cast<typename std::underlying_type<E>::type>(e);
  }

public:
  enum class object_type {
    undefined,
    file,
    executable,
    directory,
  };

  buffer_entry(const fs::path &path) : path(path) {}
  // buffer_entry(const std::string& raw_line, const std::string& name, const
  // std::string& stats_line);

  buffer_entry(const std::string &name, const std::vector<std::string> &parts);

  const std::string get_filename() const { return path.filename().string(); }

  const bool is_directory() const {
    return fs::is_directory(path);
    // return
  }

  const bool is_regular_file() const { return fs::is_regular_file(path); }

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
  // const std::string raw_line;

  object_type m_object_type;

  // Just a name, as provided by ls
  std::string name;
  std::string perms;
  std::string owner;
  std::string group;
  std::string size;
  std::string month;
  std::string day;
  std::string time;

  std::string fmt_name;
  std::string fmt_owner;
  std::string fmt_group;
  std::string fmt_size;
  std::string fmt_day;

  // Part of the line without the name
  // const std::string stats_line;

  fs::path path;
};
}

#endif /* BUFFER_ENTRY_HPP */
