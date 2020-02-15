#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <toml.hpp>

namespace derid {

    struct ThemeConfig {
        std::string selected_name = "selected";
        std::string selected_fg = "ffffff";
        std::string selected_bg = "3f88c5";

        std::string executable_name = "executable";
        std::string executable_fg = "e3655b";
        std::string executable_bg = "000000";

        std::string directory_name = "directory";
        std::string directory_fg = "ffba08";
        std::string directory_bg = "000000";

        std::string background_name = "background";
        std::string background_fg = "ffffff";
        std::string background_bg = "000000";

        std::string current_path_name = "current_path";
        std::string current_path_fg = "cbff8c";
        std::string current_path_bg = "000000";

    };

class Config {
public:
    void Read();

    const ThemeConfig& Theme() const;

private:
    void ParseTheme(const toml::value& data);
    ThemeConfig theme_config_;
};

} // namespace derid

#endif
