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
