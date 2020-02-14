#include "list.hpp"

namespace derid {
namespace widget {

List::List(const Pos &pos, int items_shown)
    : pos_(pos), index_(0), start_(0), items_shown_(items_shown),
      half_items_shown_(items_shown / 2)

{
    Refresh(buffer_.get_absolute(buffer_.get_current_path()));
}

void List::Refresh(const std::string &dir) {
    buffer_.read_dir(dir);
    index_ = start_ = 0;
}

bool List::Next() {
    // Return true of list needs to be refreshed
    int size = buffer_.entries.size();

    if (index_ + start_ >= size - 1)
        return false;

    index_ < size - 1 ? index_++ : size - 1;

    if (index_ == items_shown_) {
        start_ += half_items_shown_;

        int idx = 0;

        if (size - start_ >= items_shown_) {
            idx = half_items_shown_;
        } else {
            const int left_out = (size % items_shown_);

            idx = (size - start_) - left_out +
                  (left_out > half_items_shown_ ? half_items_shown_ : 0);
        }

        index_ = idx;
    }

    return true;
}

bool List::Prev() {
    if (index_ + start_ <= 0)
        return false;

    index_--;

    if (index_ == -1 && start_ >= half_items_shown_) {
        start_ -= half_items_shown_;
        index_ = half_items_shown_ - 1;
    }

    return true;
}

bool List::Enter() {
    const auto p = buffer_.get_entry_by_index(start_ + index_);

    auto np = fs::path(buffer_.current);

    if (buffer_.current == "/") {
        np = "/" + p;
    } else {
        np += "/" + p;
    }

    if (buffer_.is_file(np)) {
        return false;
    }

    const auto abs = buffer_.get_absolute(np);

    Refresh(abs);

    return true;
}

bool List::JumpBack() {
    Refresh(buffer_.get_absolute(buffer_.current.parent_path()));

    return true;
}

    int List::ItemsShown() const {
        return items_shown_;
    }

    const Pos& List::Position() const {
        return pos_;
    }

    int List::Index() const {
        return index_;
    }
    int List::Start() const {
        return start_;
    }

    const buffer& List::Buffer() const {
        return buffer_;
    }

} // namespace widget
} // namespace derid
