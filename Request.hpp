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
    st_body_feed,
    st_body_ready
};

class Request {
public:
    Request(void);
    ~Request(void);
    Request(const Request & other);
    Request & operator=(const Request & other);

    bool                isHeaderParsed();
    bool                isBodyParsed();

    void                push_back(std::vector<char> buf_read);
    void                clear();

    request_states                              getState() const;
    t_start_line                                getStartLine(void) const;
    std::vector<char>                           getBuf(void) const;
    std::vector<char> &                         getBuf(void);
    std::vector<char>                           getBody(void) const;
    std::vector<char> &                         getBody(void);
    const std::map<std::string, std::string> &  getHeader() const;
    std::string &                               getHeaderField(const std::string & key);

private:
    request_states                      _request_state;
    t_start_line                        _start_line;
    std::map<std::string, std::string>  _header;
    std::vector<char>                   _buf;
    std::vector<char>                   _body;

    bool                                _parseHeader();
    void                                _setBody();
    void                                _deleteHeaderInBuf();
    void                                _printBuf(std::string msg);
};

std::ostream & operator<<(std::ostream & o, Request const & other);

#endif
