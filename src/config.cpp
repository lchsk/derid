#include <iostream>

#include <boost/filesystem.hpp>

#include "config.hpp"

namespace derid {
namespace fs = boost::filesystem;

Config::Config()
    : theme_filename_("theme.toml"), settings_filename_("settings.toml") {
    HandleDefaultConfigDir();
}

const std::string &Config::GetConfigDir() const { return config_dir_; }

void Config::SetConfigDir(const std::string &dir) { config_dir_ = dir; }

void Config ::SetThemeFilename(const std::string &filename) {
    theme_filename_ = filename;
}

void Config::Read() {
    ReadTheme();
    ReadSettings();
}

const ThemeConfig &Config::Theme() const { return theme_config_; }

const SettingsConfig &Config::Settings() const { return settings_; }

void Config::ParseTheme(const toml::value &data) {
    const auto &colors = toml::find(data, "colors");

    theme_config_.selected_name =
        toml::find<std::string>(colors, "selected_name");
    theme_config_.selected_fg = toml::find<std::string>(colors, "selected_fg");
    theme_config_.selected_bg = toml::find<std::string>(colors, "selected_bg");
    theme_config_.executable_name =
        toml::find<std::string>(colors, "executable_name");
    theme_config_.executable_fg =
        toml::find<std::string>(colors, "executable_fg");
    theme_config_.executable_bg =
        toml::find<std::string>(colors, "executable_bg");
    theme_config_.directory_name =
        toml::find<std::string>(colors, "directory_name");
    theme_config_.directory_fg =
        toml::find<std::string>(colors, "directory_fg");
    theme_config_.directory_bg =
        toml::find<std::string>(colors, "directory_bg");
    theme_config_.background_name =
        toml::find<std::string>(colors, "background_name");
    theme_config_.background_fg =
        toml::find<std::string>(colors, "background_fg");
    theme_config_.background_bg =
        toml::find<std::string>(colors, "background_bg");
    theme_config_.current_path_name =
        toml::find<std::string>(colors, "current_path_name");
    theme_config_.current_path_fg =
        toml::find<std::string>(colors, "current_path_fg");
    theme_config_.current_path_bg =
        toml::find<std::string>(colors, "current_path_bg");
}

void Config::ParseSettings(const toml::value &data) {
    const auto &main = toml::find(data, "main");

    settings_.theme = toml::find<std::string>(main, "theme");
}

void Config::ReadTheme() {
    toml::value theme;

    const std::string full_path = config_dir_ + theme_filename_;

    if (!fs::exists(full_path) || !fs::is_regular_file(full_path)) {
        return;
    }

    try {
        theme = toml::parse(full_path);

        try {
            ParseTheme(theme);
        } catch (const std::exception &e) {
            std::cerr << "Error when reading theme: " << e.what() << std::endl;
        }
    } catch (const std::runtime_error &e) {
        std::cerr << "Cannot open the config file" << std::endl;
    } catch (const toml::syntax_error &e) {
        std::cerr << "Invalid config file: " << e.what() << std::endl;
    }
}

void Config::ReadSettings() {
    toml::value settings;

    const std::string full_path = config_dir_ + settings_filename_;

    if (!fs::exists(full_path) || !fs::is_regular_file(full_path)) {
        return;
    }

    try {
        settings = toml::parse(full_path);

        try {
            ParseSettings(settings);
        } catch (const std::exception &e) {
            std::cerr << "Error when reading settings: " << e.what()
                      << std::endl;
        }
    } catch (const std::runtime_error &e) {
        std::cerr << "Cannot open the settings file" << std::endl;
    } catch (const toml::syntax_error &e) {
        std::cerr << "Invalid settings file: " << e.what() << std::endl;
    }
}

void Config::HandleDefaultConfigDir() {
    const char *home_env_var = std::getenv("HOME");

    if (!home_env_var) {
        return;
    }

    const std::string home_dir = std::string(home_env_var);

    if (home_dir.size() < 1) {
        return;
    }

    std::string separator = "/";

    if (home_dir[home_dir.size() - 1] == '/') {
        separator = "";
    }

    const auto config_dir = fs::path(home_dir + separator + ".config/derid/");

    if (fs::exists(config_dir) && fs::is_directory(config_dir)) {
        config_dir_ = config_dir.string();
    }
}

} // namespace derid
