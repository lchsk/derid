#include <stdexcept>

#include "colors.hpp"

namespace derid {

    ColorPair::ColorPair(const std::string& name, const std::string& fg, const std::string& bg):

    name_(name),
    fg_(HexColorToCursesColor(fg)),
    bg_(HexColorToCursesColor(bg))
    {

    }

CursesColor HexColorToCursesColor(std::string hex_color) {
    // Length is 7 if includes the hash
    if (hex_color.length() > 7) {
        throw std::runtime_error("hex_color is too long");
    }

    if (hex_color[0] == '#') {
        hex_color.erase(0, 1);
    }

    const std::string r = hex_color.substr(0, 2);
    const std::string g = hex_color.substr(2, 2);
    const std::string b = hex_color.substr(4, 2);

    CursesColor cursesColor;

    cursesColor.r = std::stoul(r, nullptr, 16) * 1000 / 255;
    cursesColor.g = std::stoul(g, nullptr, 16) * 1000 / 255;
    cursesColor.b = std::stoul(b, nullptr, 16) * 1000 / 255;

    return cursesColor;
}


} // namespace derid
