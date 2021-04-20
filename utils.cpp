#include "utils.hpp"

namespace utils {
    std::vector<char> read_file(std::string filename) {
        std::vector<char> buf;
        int     fd;
        char    *line;
        if ((fd = open(filename.c_str(), O_RDONLY)) < 0)
            throw std::runtime_error(std::string("open: ") + strerror(errno));
        while (get_next_line(fd, &line) > 0)
        {
            std::string str(line);
            buf.insert(buf.end(), str.begin(), str.end());
        }
        std::string str(line);
        buf.insert(buf.end(), str.begin(), str.end());
        return buf;
    }
}
