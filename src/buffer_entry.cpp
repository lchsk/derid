#include "buffer_entry.hpp"

#include <iostream>

namespace derid {

BufferEntry::BufferEntry(const std::string &name,
                           const std::vector<std::string> &parts)
    : name_(name) {
    if (parts.size() < min_parts_sz_) {
        return;
    }

    perms_ = parts[ToType<InformationType>(InformationType::perms)];
    owner_ = parts[ToType<InformationType>(InformationType::owner)];
    group_ = parts[ToType<InformationType>(InformationType::group)];
    size_ = parts[ToType<InformationType>(InformationType::size)];
    date_ = parts[ToType<InformationType>(InformationType::date)];
    time_ = parts[ToType<InformationType>(InformationType::time)];

    std::istringstream ss(date_ + " " + time_);

    ss >> std::get_time(&datetime_, "%Y-%m-%d %H:%M");

    if (!ss.fail()) {
        // std::cout << std::put_time(&datetime, "%c") << '\n';
    }

    if (perms_.size() == 10) {
        switch (perms_[0]) {
        case 'd':
            buffer_entry_type_ = BufferEntryType::directory;
            break;
        case '-':
            if (perms_[3] == 'x' or perms_[6] == 'x' or perms_[9] == 'x') {
                buffer_entry_type_ = BufferEntryType::executable;
            } else {
                buffer_entry_type_ = BufferEntryType::file;
            }
            break;
        }
    } else {
        buffer_entry_type_ = BufferEntryType::undefined;
    }
}

const std::string
BufferEntry::GetFormattedDatetime(const std::string &format) const {
    std::stringstream out;

    out << std::put_time(&datetime_, format.c_str());

    return out.str();
}

    const BufferEntry::BufferEntryType BufferEntry::Type() const {
        return buffer_entry_type_;
    }

    const std::string& BufferEntry::Name() const {
        return name_;
    }
    const std::string& BufferEntry::Size() const {
        return size_;
    }
    const std::string& BufferEntry::Perms() const {
        return perms_;
    }
    const std::string& BufferEntry::Owner() const {
        return owner_;
    }
    const std::string& BufferEntry::Group() const {
        return group_;
    }

    const std::string& BufferEntry::FmtName() const {
        return fmt_name_;
    }

    void BufferEntry::SetFmtName(const std::string& name) {
        fmt_name_ = name;
    }

    const std::string& BufferEntry::FmtSize() const {
        return fmt_size_;
    }

    void BufferEntry::SetFmtSize(const std::string& size) {
        fmt_size_ = size;
    }

    const std::string& BufferEntry::FmtOwner() const {
        return fmt_owner_;
    }

    void BufferEntry::SetFmtOwner(const std::string& owner) {
        fmt_owner_ = owner;
    }

    const std::string& BufferEntry::FmtGroup() const {
        return fmt_group_;
    }

    void BufferEntry::SetFmtGroup(const std::string& group) {
        fmt_group_ = group;
    }

    const std::string& BufferEntry::FmtDatetime() const {
        return fmt_datetime_;
    }

} // namespace derid
