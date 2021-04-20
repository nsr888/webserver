#ifndef UTILS_HPP
# define UTILS_HPP
# include <fcntl.h>
# include <string>
# include <vector>
# include <cerrno>
extern "C" {
    int get_next_line(int fd, char **line);
}

namespace utils {
    std::vector<char> read_file(std::string filename);
}

#endif
