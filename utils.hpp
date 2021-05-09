#ifndef UTILS_HPP
# define UTILS_HPP
# include <unistd.h>
# include <fcntl.h>
# include <string>
# include <vector>
# include <cerrno>
# include <iostream>
# include <cstdlib>
# include <sys/time.h>

extern "C" {
    int                 get_next_line(int fd, char **line);
    void	            *ft_calloc(size_t count, size_t size);
}

namespace utils {
    std::vector<char>   read_file(std::string filename);
    std::vector<char>   read_file_raw(std::string filename);
    void                write_file_raw(std::string filename, std::vector<char> buf);
    bool                file_exists (std::string filename);
    bool                in_array(const std::string &value, const std::vector<std::string> &array);
    size_t              get_current_time_in_ms(void);
    void				ft_usleep(int ms);
    std::string			ft_strtrim(const std::string &s1, const std::string& set);
    int					ft_strchr(const std::string& str, int ch);
}

#endif
