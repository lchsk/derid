#include "buffer.hpp"

#define USE_LS

namespace derid {
    const std::string exec_shell_cmd(const std::string& cmd) {
        constexpr int buffer_size = 128;

        std::array<char, buffer_size> buffer;

        std::string result = "";

        std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);

        if (!pipe) {
            return result;
        }

        while (!feof(pipe.get())) {
            if (fgets(buffer.data(), buffer_size, pipe.get()) != nullptr)
                result += buffer.data();
        }

        return result;
    }

    std::vector<std::string> split_on_new_line(const std::string& text)
    {
        std::stringstream ss(text);
        std::string line, temp;
        std::vector<std::string> lines;

        while (std::getline(ss, temp, '\n')) {
            line = "";

            for (int i = 0; i < temp.length(); i++) {
                if (temp[i] == ' ') {
                    line += '_';
                }
                else
                    {
                        line += temp[i];
                    }
            }

            lines.push_back(line);
        }

        return lines;
    }

bool buffer::is_file(const fs::path& p)
{
    return fs::exists(p) && fs::is_regular_file(p);
}

const fs::path buffer::get_current_path()
{
    return boost::filesystem::current_path();
}

std::string buffer::get_absolute(const fs::path& p)
{
    return fs::absolute(p).string();
}

std::string buffer::get_absolute(int index)
{
    return fs::absolute(paths[index]).string();
}

const fs::path buffer::get_path_by_index(int index)
{
    return fs::absolute(paths[index]);
}

void buffer::read_dir(const std::string& dir)
{
#ifdef USE_LS
    std::stringstream ss;
    ss << "ls -lh "
       << dir;

    const std::string s = derid::exec_shell_cmd(ss.str());

    list = derid::split_on_new_line(s);

#else

    BOOST_LOG_TRIVIAL(debug) << dir;

    try {
        fs::path p(dir);
        current = p;

        if (!fs::exists(p))
            return;

        list.clear();
        paths.clear();
        entries.clear();

        this->dir = dir;

        for (fs::directory_entry& x : fs::directory_iterator(p)) {
            paths.push_back(x.path()); // remove
            entries.push_back(derid::buffer_entry(x.path()));
        }
    } catch (const fs::filesystem_error& e) {
        BOOST_LOG_TRIVIAL(debug) << e.what();

        return;
    }
    #endif
}

} // namespace derid
