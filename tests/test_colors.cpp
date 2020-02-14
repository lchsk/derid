#include "gtest/gtest.h"

#include "../src/colors.hpp"

using namespace derid;

TEST(HexColorWithoutHashToCurses, black) {
    const auto curses_color = HexColorToCursesColor("000000");

    ASSERT_EQ(curses_color.r, 0);
    ASSERT_EQ(curses_color.g, 0);
    ASSERT_EQ(curses_color.b, 0);
}

TEST(HexColorWithoutHashToCurses, white) {
    const auto curses_color = HexColorToCursesColor("ffffff");

    ASSERT_EQ(curses_color.r, 1000);
    ASSERT_EQ(curses_color.g, 1000);
    ASSERT_EQ(curses_color.b, 1000);
}

TEST(HexColorWithoutHashToCurses, selective_yellow) {
    const auto curses_color = HexColorToCursesColor("ffba08");

    ASSERT_EQ(curses_color.r, 1000);
    ASSERT_EQ(curses_color.g, 729);
    ASSERT_EQ(curses_color.b, 31);
}

TEST(HexColorWithHashToCurses, black) {
    const auto curses_color = HexColorToCursesColor("#000000");

    ASSERT_EQ(curses_color.r, 0);
    ASSERT_EQ(curses_color.g, 0);
    ASSERT_EQ(curses_color.b, 0);
}

TEST(HexColorWithHashToCurses, string_is_too_long) {
    ASSERT_THROW(HexColorToCursesColor("#00000000000"), std::runtime_error);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
