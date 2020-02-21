#include "gtest/gtest.h"

#include "../src/config.hpp"

using namespace derid;

// Theme

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
    config.SetConfigDir("../tests/test_data/.config/derid/");
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
    config.SetConfigDir("../tests/test_data/.config/derid/");

    // If the file is missing values, it should read default values
    config.SetThemeFilename("missing_keys.toml");
    config.Read();

    const auto& theme = config.Theme();

    // Keys present should be read correctly
    ASSERT_EQ(theme.selected_name, "test");
    ASSERT_EQ(theme.selected_fg, "555555");
    ASSERT_EQ(theme.selected_bg, "111111");

    // Other keys should use default values
    ASSERT_EQ(theme.executable_name, "executable");
    ASSERT_EQ(theme.executable_fg, "e3655b");
    ASSERT_EQ(theme.executable_bg, "000000");
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

// Settings

TEST(Settings, read_settings_success) {
    Config config;
    config.SetConfigDir("../tests/test_data/config/");
    config.Read();

    const auto& settings = config.Settings();

    ASSERT_EQ(settings.theme, "default");
}

TEST(Settings, handle_default_config_dir_without_home_env) {
    setenv("HOME", "", true);

    Config config;

    ASSERT_EQ(config.GetConfigDir(), "");
}

TEST(Settings, handle_default_config_dir_with_non_existent_home_dir) {
    setenv("HOME", "abcdef", true);

    Config config;

    ASSERT_EQ(config.GetConfigDir(), "");
}

TEST(Settings, handle_default_config_dir_with_home_env) {
    setenv("HOME", "../tests/test_data", true);

    Config config;

    ASSERT_EQ(config.GetConfigDir(), "../tests/test_data/.config/derid/");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
