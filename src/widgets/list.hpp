#ifndef LIST_HPP
#define LIST_HPP

#include "../buffer.hpp"
#include "../pos.hpp"

namespace derid {
namespace widget {
class List {
  public:
    List(const Pos &p, int items_shown);

    // OUT
    void Refresh(const std::string &dir);

    bool Next();
    bool Prev();

    // OUT OF HERE
    bool Enter();

    // OUT OF HERE
    bool JumpBack();

    int ItemsShown() const;
    const Pos& Position() const;
    int Index() const;
    int Start() const;
    const buffer& Buffer() const;

private:
    const int items_shown_;
    const int half_items_shown_;
    const Pos pos_;
    int index_;
    int start_;
    buffer buffer_;
};
} // namespace widget
} // namespace derid
#endif
