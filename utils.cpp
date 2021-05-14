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

    std::map<std::string, std::string>
    parseBufToHeaderMap(const std::map<std::string, std::string> & header,
            const std::vector<char> & buf)
    {
        // All headers in map (key - value)
        std::vector<char>::const_iterator head = buf.begin();
        std::vector<char>::const_iterator tail = head;
        std::map<std::string, std::string> header_new = header;

        while (head != buf.end() && *head != '\r')
        {
            while (tail != buf.end() && *tail != '\r')
                ++tail;
            std::vector<char>::const_iterator separator = head;
            while (separator != buf.end() && separator != tail && *separator != ':')
                ++separator;
            if (separator == tail)
                break;
            std::string key(head, separator);
            std::vector<char>::const_iterator value = ++separator;
            while (value != tail && (*value == ' ' || *value == ':'))
                ++value;
            header_new[key] = std::string(value, tail);
            while (tail != buf.end() && (*tail == '\r' || *tail == '\n'))
                ++tail;
            head = tail;
        }
        return header_new;
    }
    std::string base64encode(std::vector<char> buf) {
        const char base64Keys[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
        std::string output;
        char chr1, chr2, chr3;
        int enc1, enc2, enc3, enc4;
        size_t i = 0;

        size_t buf_len = buf.size();
        output.reserve(4 * buf_len / 3);
        while (i < buf_len)
        {
            chr1 = (i < buf_len) ? buf[i++] : 0;
            chr2 = (i < buf_len) ? buf[i++] : 0;
            chr3 = (i < buf_len) ? buf[i++] : 0;

            enc1 = chr1 >> 2;
            enc2 = ((chr1 & 0x3) << 4) | (chr2 >> 4);
            enc3 = ((chr2 & 0xF) << 2) | (chr3 >> 6);
            enc4 = chr3 & 0x3F;

            if (chr2 == 0)
            {
                enc3 = 64;
                enc4 = 64;
            }
            else if (chr3 == 0)
            {
                enc4 = 64;
            }
            output += base64Keys[enc1];
            output += base64Keys[enc2];
            output += base64Keys[enc3];
            output += base64Keys[enc4];
        }
        return output;
    }
    std::string base64decode(const std::string & s) {
        const char base64Keys[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        std::string output;
        output.clear();
        output.reserve(3 * s.length() / 4);
        std::vector<int> T(256, -1);
        for (int i = 0; i < 64; ++i)
        {
            T[base64Keys[i]] = i;
        }
        int val = 0;
        int valb = -8;
        for (size_t i = 0; i < s.length(); ++i)
        {
            char c = s[i];
            if (T[c] == -1) break;
            val = (val << 6) + T[c];
            valb += 6;
            if (valb >= 0)
            {
                output += char((val >> valb) & 0xFF);
                valb -= 8;
            }
        }
        return output;
    }
}
