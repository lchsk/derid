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

// TODO: Move somewhere else
const std::string ExecShellCmd(const std::string &);
std::vector<std::string> SplitOnNewLine(const std::string &);

class Buffer {
  public:
    void ReadDir(const std::string &dir);
    bool IsFile(const fs::path &p);

    const fs::path GetCurrentPath() const;

    std::string GetAbsolute(const fs::path &p) const;
    std::string GetAbsolute(int index);

    const std::string GetEntryByIndex(int index);

    const std::vector<std::pair<std::string, std::string>>
    GetLineData(int index) const;

    const fs::path &Current() const;
    const std::vector<BufferEntry> &Entries() const;

  private:
    // const std::string GetLine(int index) const;
    const std::string format = "%perms %owner %group %size %datetime %name";

    const std::string datetime_format = "%Y-%m-%d %H:%M";

    fs::path current_;
    std::vector<fs::path> paths_;
    std::vector<BufferEntry> entries_;
};
} // namespace derid

#endif
