#include "label.hpp"

namespace derid {
    namespace widget {
        Label::Label(const Pos &pos, const std::string &text)
        : pos_(pos), text_(text), color_(-1) {}

        const Pos& Label::Position() const {
            return pos_;
        }

        const std::string& Label::Text() const {
            return text_;
        }

        void Label::SetText(const std::string& text) {
            text_ = text;
        }

        int Label::Color() const {
            return color_;
        }

        void Label::SetColor(int color) {
            color_ = color;
        }

    }
} // namespace derid
