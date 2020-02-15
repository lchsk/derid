#include <iostream>

#include "config.hpp"

namespace derid {
    void Config::Read() {

        toml::value data;

        try {
            data = toml::parse("theme.toml");

            try {
                ParseTheme(data);
            } catch (const std::exception &e) {
                std::cerr << "Error when reading theme: " << e.what() << std::endl;
            }
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Cannot open the config file" << std::endl;
        }
        catch (const toml::syntax_error& e) {
            std::cerr << "Invalid config file: " << e.what() << std::endl;
        }

    }

    const ThemeConfig& Config::Theme() const {
        return theme_config_;
    }

    void Config::ParseTheme(const toml::value& data) {
        const auto& colors = toml::find(data, "colors");

        theme_config_.selected_name = toml::find<std::string>(colors, "selected_name");
        theme_config_.selected_fg = toml::find<std::string>(colors, "selected_fg");
        theme_config_.selected_bg = toml::find<std::string>(colors, "selected_bg");
        theme_config_.executable_name = toml::find<std::string>(colors, "executable_name");
        theme_config_.executable_fg = toml::find<std::string>(colors, "executable_fg");
        theme_config_.executable_bg = toml::find<std::string>(colors, "executable_bg");
        theme_config_.directory_name = toml::find<std::string>(colors, "directory_name");
        theme_config_.directory_fg = toml::find<std::string>(colors, "directory_fg");
        theme_config_.directory_bg = toml::find<std::string>(colors, "directory_bg");
        theme_config_.background_name = toml::find<std::string>(colors, "background_name");
        theme_config_.background_fg = toml::find<std::string>(colors, "background_fg");
        theme_config_.background_bg = toml::find<std::string>(colors, "background_bg");
        theme_config_.current_path_name = toml::find<std::string>(colors, "current_path_name");
        theme_config_.current_path_fg = toml::find<std::string>(colors, "current_path_fg");
        theme_config_.current_path_bg = toml::find<std::string>(colors, "current_path_bg");


    }
} // namespace derid
