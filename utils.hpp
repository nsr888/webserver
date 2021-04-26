#ifndef UTILS_HPP
# define UTILS_HPP
# include <unistd.h>
# include <fcntl.h>
# include <string>
# include <vector>
# include <cerrno>
# include <iostream>
# include <cstdlib>

extern "C" {
    int get_next_line(int fd, char **line);
}

namespace utils {
    std::vector<char> read_file(std::string filename);
}

std::string					ft_strtrim(const std::string &s1, const std::string& set);
int							ft_strchr(const std::string& str, int ch);

#endif
