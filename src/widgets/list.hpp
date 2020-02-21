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
    const Pos &Position() const;
    int Index() const;
    int Start() const;
    const Buffer &GetBuffer() const;

  private:
    const Pos pos_;
    int index_;
    int start_;
    const int items_shown_;
    const int half_items_shown_;

    Buffer buffer_;
};
} // namespace widget
} // namespace derid
#endif
