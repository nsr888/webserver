#include <unistd.h>
#include <sys/stat.h>
#include "utils.hpp"
#include <cstring> // for linux

namespace utils {
    std::vector<char> read_file(std::string filename) {
        std::vector<char>   buf;
        int                 fd;
        char*               line;

        if ((fd = open(filename.c_str(), O_RDONLY)) < 0)
            throw std::runtime_error(std::string("open: ") + strerror(errno));
        while (get_next_line(fd, &line) > 0)
            buf.insert(buf.end(), line, line + std::strlen(line));
        buf.insert(buf.end(), line, line + std::strlen(line));
        close(fd);
        return buf;
    }

    std::vector<char> read_file_raw(std::string filename) {
        std::vector<char>   buf;
        int                 fd;
        char                line[5];
        int                 m;

        if ((fd = open(filename.c_str(), O_RDONLY)) < 0)
            throw std::runtime_error(std::string("open: ") + strerror(errno));
        while ((m = read(fd, line, 5)) > 0)
        {
            if (m == -1)
                throw std::runtime_error(std::string("write: ") + strerror(errno));
            buf.insert(buf.end(), line, line + m);
        }
        close(fd);
        return buf;
    }

    void write_file_raw(std::string filename, std::vector<char> buf) {
        int                 fd;
        int                 m;
        int                 offset;

        offset = 0;
        m = 0;
        if ((fd = open(filename.c_str(), O_TRUNC | O_WRONLY | O_CREAT, 0644)) < 0)
            throw std::runtime_error(std::string("open: ") + strerror(errno));
        std::vector<char>::iterator it = buf.begin();
        while (it != buf.end())
        {
            if(lseek(fd, 0, SEEK_CUR) == -1)
                throw std::runtime_error(std::string("lseek: ") + strerror(errno));
            if ((m = write(fd, &buf[offset], 1)) != 1)
                throw std::runtime_error(std::string("write: ") + strerror(errno));
            offset = offset + m;
            ++it;
        }
        if (m == -1)
            throw std::runtime_error(std::string("write: ") + strerror(errno));
        close(fd);
    }

    bool file_exists (std::string filename) {
      struct stat   buffer;   
      return (stat (filename.c_str(), &buffer) == 0);
    }

    bool in_array(const std::string &value, const std::vector<std::string> &array)
    {
        for ( std::vector<std::string>::const_iterator it = array.begin();
            it != array.end(); ++it)
        {
            if (value == *it)
                return true;
        }
        return false;
    }

    size_t get_current_time_in_ms(void)
    {
        struct timeval	tv;
        size_t          time_in_mill;

        gettimeofday(&tv, NULL);
        time_in_mill = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        return (time_in_mill);
    }

    void				ft_usleep(int ms)
    {
        size_t          start_time;
        size_t          end_time;
        int				elapsed_time;

        start_time = get_current_time_in_ms();
        end_time = get_current_time_in_ms();
        elapsed_time = end_time - start_time;
        while (elapsed_time < ms)
        {
            usleep(10);
            end_time = get_current_time_in_ms();
            elapsed_time = end_time - start_time;
        }
    }

    std::string	ft_strtrim(const std::string &s1, const std::string& set)
    {
        size_t	start;
        size_t	end;
        size_t	index = 0;

        while (s1[index] && utils::ft_strchr(set, s1[index]) != -1)
            index++;
        start = index;
        end = s1.length();
        while (end && utils::ft_strchr(set, s1[end]) != -1)
            --end;
        return s1.substr(start, end + 1);
    }

    int	ft_strchr(const std::string& str, int ch)
    {
        char			*src;
        int	index;

        src = (char *)str.c_str();
        index = 0;
        while (src[index] != 0)
            if (src[index++] == ch)
                return (index);
        if (src[index] == ch)
            return (index);
        return (-1);
    }
}
