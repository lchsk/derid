#ifndef COLORS_HPP
#define COLORS_HPP

#include <string>
#include <vector>

namespace derid {

// Each value is in the range of 0-1000
struct CursesColor {
    unsigned short r;
    unsigned short g;
    unsigned short b;
};

    struct ColorPair {
        // Hex values
        // std::string selected_fg;
        // std::string selected_bg;

        ColorPair(const std::string& name, const std::string& fg, const std::string& bg);
        ColorPair() {};

        std::string name_;

        // CursesColors
        CursesColor fg_;
        CursesColor bg_;
    };

    struct ColorTheme {
        ColorPair selected;
        ColorPair executable;
        ColorPair directory;
        ColorPair background;
        ColorPair current_path;

        const std::vector<ColorPair> GetColors() const {
            return {selected, executable, directory, background, current_path};
        }
    };

    CursesColor HexColorToCursesColor(std::string hex_color);
} // namespace derid

#endif
