#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <iostream>
# include <string>
# include <vector>
# include <map>
extern "C" {
    int ft_atoi(const char *str);
}

typedef struct s_start_line{
    std::string     method;
    std::string     request_target;
    std::string     http_version;
} t_start_line;

enum request_states {
    st_header_feed,
    st_header_found,
    st_header_parsed,
    st_body_feed,
    st_body_ready
};

class Request {
public:
    Request(void);
    ~Request(void);
    Request(const Request & other);
    Request & operator=(const Request & other);

    void                                appendBuf(const char * buf);
    void                                parseHeader();
    void                                checkBodyFull();
    void                                clear();

    request_states                      getState() const;
    t_start_line                        getStartLine(void) const;
    std::vector<char>                   getBuf(void) const;
    std::vector<char> &                 getBuf(void);
    std::vector<char>                   getBody(void) const;
    std::vector<char> &                 getBody(void);
    std::map<std::string, std::string>  getHeader() const;
    std::string                         getHeaderBuf() const;
    std::string &                       getHeaderField(const std::string & key);

private:
    request_states                      _request_state;
    t_start_line                        _start_line;
    std::string                         _header_buf;
    std::map<std::string, std::string>  _header;
    std::vector<char>                   _buf;
    std::vector<char>                   _body;

    void                                _delBufHeader();
    bool                                _findHeader();
    void                                _debugPrint(std::string msg);
};

std::ostream & operator<<(std::ostream & o, Request const & other);

#endif
