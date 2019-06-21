#ifndef BUFFER_ENTRY_HPP
#define BUFFER_ENTRY_HPP

#include <iomanip>
#include <string>
#include <vector>

namespace derid {
class buffer_entry {
  enum class entry_type : unsigned int {
    perms = 0,
    owner = 2,
    group = 3,
    size = 4,

    date = 5,
    time = 6,

    // month = 5,
    // day = 6,
    // time = 7,
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

  buffer_entry(const std::string &name, const std::vector<std::string> &parts);

    const std::string get_formatted_datetime(const std::string& format) const;

  object_type m_object_type;

  // Just a name, as provided by ls
  std::string name;
  std::string perms;
  std::string owner;
  std::string group;
  std::string size;
    std::string date;
    std::string time;

    std::tm datetime;

    // Separators on the right hand size

    std::string sep_perms;
    std::string sep_owner;
    std::string sep_group;
    std::string sep_size;

    // Formats

  std::string fmt_name;
  std::string fmt_owner;
  std::string fmt_group;
  std::string fmt_size;
  std::string fmt_datetime;
};
}

#endif /* BUFFER_ENTRY_HPP */
