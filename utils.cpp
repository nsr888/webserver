#include <unistd.h>
#include <sys/stat.h>
#include "utils.hpp"

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
        if ((fd = open(filename.c_str(), O_TRUNC | O_WRONLY | O_CREAT)) < 0)
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
}

std::string	ft_strtrim(const std::string &s1, const std::string& set)
{
	size_t	start;
	size_t	end;
	size_t	index = 0;

	while (s1[index] && ft_strchr(set, s1[index]) != -1)
		index++;
	start = index;
	end = s1.length();
	while (end && ft_strchr(set, s1[end]) != -1)
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
