#include <sys/stat.h> // umask
#include <unistd.h>   // execve, readlink

#include "config.hpp"
#include "files.hpp"

#include <cassert>
#include <unistd.h>

extern char **environ;

namespace derid {
void Open(const std::string &path) {
    pid_t pid;

    switch (pid = fork()) {
    case -1:
        // TODO: Report error
        break;

    case 0: // Child
        /* Change the file mode mask */
        umask(0);

        if (setsid() < 0) {
            // TODO
        }

        const std::string dir = GetHomeDir();

        if (chdir(dir.size() > 0 ? dir.c_str() : "/tmp") < 0) {
            // TODO
        }

        /* Redirect standard files to /dev/null */
        if (freopen("/dev/null", "r", stdin) == nullptr) {
            // TODO
        }

        if (freopen("/dev/null", "w", stdout) == nullptr) {
            // TODO
        }
        if (freopen("/dev/null", "w", stderr) == nullptr) {
            // TODO
        }

        const char *const argv[] = {"xdg-open", path.c_str(), nullptr};

        execvpe("xdg-open", const_cast<char *const *>(argv), environ);
    }
}

} // namespace derid
