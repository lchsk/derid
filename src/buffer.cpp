#include "buffer.hpp"

namespace derid {
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
    BOOST_LOG_TRIVIAL(debug) << dir;

    try {
        fs::path p(dir);
        current = p;

        if (!fs::exists(p))
            return;

        list.clear();
        paths.clear();

        this->dir = dir;

        for (fs::directory_entry& x : fs::directory_iterator(p)) {
            list.push_back(x.path().filename().string());
            paths.push_back(x.path());
        }
    } catch (const fs::filesystem_error& e) {
        BOOST_LOG_TRIVIAL(debug) << e.what();

        return;
    }
}

} // namespace derid
