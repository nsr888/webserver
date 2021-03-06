#ifndef UTILS_HPP
# define UTILS_HPP
# include <unistd.h>
# include <fcntl.h>
# include <string>
# include <map>
# include <vector>
# include <cerrno>
# include <iostream>
# include <cstdlib>
# include <sys/time.h>
# include "Setting.hpp"

extern "C" {
    int                 get_next_line(int fd, char **line);
    void*               ft_calloc(size_t count, size_t size);
    char*               ft_itoa(int n);
}

namespace utils {
    std::vector<char>   read_file(std::string filename);
    std::vector<char>   read_file_raw(std::string filename);
    void                write_file_raw(std::string filename, std::vector<char> buf);
    bool                file_exists (std::string filename);
    int                 file_dir_exists (std::string filename);
    bool                in_array(const std::string &value, const std::vector<std::string> &array);
    size_t              get_current_time_in_ms(void);
    void				ft_usleep(int ms);
    std::string			ft_strtrim(const std::string &s1, const std::string& set);
    std::string     	ft_strtrim2(const std::string &s1, const std::string& set);
    int					ft_strchr(const std::string& str, int ch);
    std::map<std::string, std::string>  parseBufToHeaderMap(
                            const std::map<std::string, std::string> & header,
                            const std::vector<char> & buf);
    std::string         base64encode(std::vector<char> buf);
    std::string         base64decode(const std::string & s);
    std::string         to_string(int n);
    void                log(Setting & config, const std::string & filename,
                            const std::string & msg, int n = -1);
    std::string         get_current_time_fmt();
    void                e_throw(const std::string & msg, const std::string & filename, int line);

    const char YEL[] = "\033[0;33m";
    const char RED[] = "\033[0;31m";
    const char GRN[] = "\033[0;32m";
    const char BLU[] = "\033[0;34m";
    const char MAG[] = "\033[0;35m";
    const char CYN[] = "\033[0;36m";
    const char WHT[] = "\033[0;37m";
    const char GRA[] = "\033[0;30m";
    const char RES[] = "\033[0m";
    const char OK[] = "[\033[0;32mOK\033[0m] ";
    const char PASS[] = "[\033[0;30mpass\033[0m] ";
}

#endif
