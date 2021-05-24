#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include "utils.hpp"
#include "Setting.hpp"
#include <cstring> // for linux

namespace utils {
    std::vector<char> read_file(std::string filename) {
        std::vector<char>   buf;
        int                 fd;
        char*               line;

        if ((fd = open(filename.c_str(), O_RDONLY)) < 0)
            utils::e_throw("open", __FILE__, __LINE__);
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
            utils::e_throw("open", __FILE__, __LINE__);
        while ((m = read(fd, line, 5)) > 0)
        {
            if (m == -1)
                utils::e_throw("read", __FILE__, __LINE__);
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
            utils::e_throw("open", __FILE__, __LINE__);
        std::vector<char>::iterator it = buf.begin();
        while (it != buf.end())
        {
            if(lseek(fd, 0, SEEK_CUR) == -1)
                utils::e_throw("lseek", __FILE__, __LINE__);
            if ((m = write(fd, &buf[offset], buf.size())) < 0)
                utils::e_throw("write_file_raw", __FILE__, __LINE__);
            offset = offset + m;
            it = it + m;
        }
        close(fd);
    }

    bool file_exists (std::string filename) {
      struct stat   buffer;   
      return (stat (filename.c_str(), &buffer) == 0);
    }

    int file_dir_exists (std::string filename) {
        struct stat   info;   
        if( stat(filename.c_str(), &info ) != 0 )
            return 0;
        else if( info.st_mode & S_IFDIR )
            return 2;
        else
            return 1;
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

    std::string	ft_strtrim2(const std::string &s1, const std::string& set)
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
        return s1.substr(start, end);
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
    std::string to_string(int n)
    {
        char * loc_num = ft_itoa(n);
        std::string msg = std::string(loc_num);
        free(loc_num);
        return msg;
    }
    std::string get_current_time_fmt()
    {
        struct timeval tv;
        time_t nowtime;
        struct tm *nowtm;
        char tmbuf[64];

        gettimeofday(&tv, NULL);
        nowtime = tv.tv_sec;
        nowtm = localtime(&nowtime);
        strftime(tmbuf, sizeof(tmbuf), "%Y-%m-%d %H:%M:%S", nowtm);
        std::string str(tmbuf);
        return str;
    }

    void log_print_template(const char *color, const std::string & filename, const std::string & msg, int n)
    {
        std::cout << color;
        std::cout << get_current_time_fmt() << " ";
        std::cout << filename << ":" << RES << " " << msg;
        if (n != -1) std::cout << n;
        std::cout << std::endl;
    }

    void log(Setting & config, const std::string & filename,
            const std::string & msg, int n)
    {
        if (filename == "EventLoop.cpp" && config.getDebugLevel() == -9)
            log_print_template(GRA, filename, msg, n);
        if (filename == "Response.cpp" && config.getDebugLevel() > 1)
            log_print_template(CYN, filename, msg, n);
        if (filename == "ProcessMethod.cpp" && config.getDebugLevel() > 1)
            log_print_template(MAG, filename, msg, n);
        if (filename == "Client.cpp" && config.getDebugLevel() > 2)
            log_print_template(BLU, filename, msg, n);
        if (filename == "HTTP HEADER" && config.getDebugLevel() == -1)
            log_print_template(GRN, filename, msg, n);
    }

    void e_throw(const std::string & msg, const std::string & filename, int line)
    {
        throw std::runtime_error(msg + ": " + strerror(errno) 
                + " at "+ filename + ":" + to_string(line));
    }
}
