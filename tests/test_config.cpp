#include "gtest/gtest.h"

#include "../src/config.hpp"

using namespace derid;

void AssertDefaultThemeValues(const ThemeConfig& theme) {
   ASSERT_EQ(theme.selected_name, "selected");
    ASSERT_EQ(theme.selected_fg, "ffffff");
    ASSERT_EQ(theme.selected_bg, "3f88c5");
    ASSERT_EQ(theme.executable_name, "executable");
    ASSERT_EQ(theme.executable_fg, "e3655b");
    ASSERT_EQ(theme.executable_bg, "000000");
    ASSERT_EQ(theme.directory_name, "directory");
    ASSERT_EQ(theme.directory_fg, "ffba08");
    ASSERT_EQ(theme.directory_bg, "000000");
    ASSERT_EQ(theme.background_name, "background");
    ASSERT_EQ(theme.background_fg, "ffffff");
    ASSERT_EQ(theme.background_bg, "000000");
    ASSERT_EQ(theme.current_path_name, "current_path");
    ASSERT_EQ(theme.current_path_fg, "cbff8c");
    ASSERT_EQ(theme.current_path_bg, "000000");

}

TEST(ThemeConfig, read_theme_config_success) {
    Config config;
    config.SetConfigDir("../tests/test_data/config/");
    config.Read();

    const auto& theme = config.Theme();

    ASSERT_EQ(theme.selected_name, "selected");
    ASSERT_EQ(theme.selected_fg, "123456");
    ASSERT_EQ(theme.selected_bg, "abcdef");
    ASSERT_EQ(theme.executable_name, "executable");
    ASSERT_EQ(theme.executable_fg, "123456");
    ASSERT_EQ(theme.executable_bg, "abcdef");
    ASSERT_EQ(theme.directory_name, "directory");
    ASSERT_EQ(theme.directory_fg, "123456");
    ASSERT_EQ(theme.directory_bg, "abcdef");
    ASSERT_EQ(theme.background_name, "background");
    ASSERT_EQ(theme.background_fg, "123456");
    ASSERT_EQ(theme.background_bg, "abcdef");
    ASSERT_EQ(theme.current_path_name, "current_path");
    ASSERT_EQ(theme.current_path_fg, "123456");
    ASSERT_EQ(theme.current_path_bg, "abcdef");

}

TEST(ThemeConfig, read_theme_config_missing_keys) {
    Config config;
    config.SetConfigDir("../tests/test_data/config/");

    // If the file is missing values, it should read default values
    config.SetThemeFilename("missing_keys.toml");
    config.Read();

    AssertDefaultThemeValues(config.Theme());
}


TEST(ThemeConfig, read_theme_config_file_doesnt_exist) {
    Config config;
    config.SetConfigDir("../tests/test_data/does_not_exist/");
    config.Read();

    AssertDefaultThemeValues(config.Theme());
}

TEST(ThemeConfig, read_theme_config_invalid_syntax) {
    Config config;
    config.SetConfigDir("../tests/test_data/config/");
    config.SetThemeFilename("invalid_syntax.toml");
    config.Read();

    AssertDefaultThemeValues(config.Theme());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
