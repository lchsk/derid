#ifndef BUFFER_ENTRY_HPP
#define BUFFER_ENTRY_HPP

#include <iomanip>
#include <string>
#include <vector>

namespace derid {
class BufferEntry {
  public:
    enum class BufferEntryType {
        undefined,
        file,
        executable,
        directory,
    };

    BufferEntry(const std::string &name, const std::vector<std::string> &parts);

    const BufferEntry::BufferEntryType Type() const;
    const std::string GetFormattedDatetime(const std::string &format) const;

    const std::string &Name() const;
    const std::string &Size() const;
    const std::string &Perms() const;
    const std::string &Owner() const;
    const std::string &Group() const;

    const std::string &FmtName() const;
    void SetFmtName(const std::string &name);
    const std::string &FmtSize() const;
    void SetFmtSize(const std::string &size);
    const std::string &FmtOwner() const;
    void SetFmtOwner(const std::string &owner);
    const std::string &FmtGroup() const;
    void SetFmtGroup(const std::string &group);

    const std::string &FmtDatetime() const;

  private:
    enum class InformationType : unsigned int {
        perms = 0,
        owner = 2,
        group = 3,
        size = 4,

        date = 5,
        time = 6,
    };

    const std::size_t min_parts_sz_ = 8;
    BufferEntryType buffer_entry_type_;

    template <typename E> constexpr auto ToType(E e) {
        return static_cast<typename std::underlying_type<E>::type>(e);
    }

    // Just a name, as provided by ls
    std::string name_;

    std::string perms_;
    std::string owner_;
    std::string group_;
    std::string size_;
    std::string date_;
    std::string time_;

    std::tm datetime_;

    // Formats
    std::string fmt_name_;
    std::string fmt_owner_;
    std::string fmt_group_;
    std::string fmt_size_;
    std::string fmt_datetime_;
};
} // namespace derid

#endif /* BUFFER_ENTRY_HPP */
