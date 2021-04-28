#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <iostream>
# include <string>
# include <vector>
# include <map>
# include "utils.hpp"

extern "C" {
    int                 ft_atoi(const char *str);
    char				*ft_itoa(int n);
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

enum chunk_states {
    ch_header_feed,
    ch_body_feed,
    ch_body_ready
};

class Request {
public:
    Request(void);
    ~Request(void);
    Request(const Request & other);
    Request & operator=(const Request & other);

    bool                isHeaderParsed();
    bool                isBodyParsed();
    bool                isHeaderContains(std::string str);
    bool                isHeaderValid() const;

    void                push_back(std::vector<char> buf_read);
    void                setChunkState(chunk_states chunk_state);
    void                clear();

    request_states                      getState() const;
    t_start_line                        getStartLine(void) const;
    std::vector<char>                   getBuf(void) const;
    std::vector<char> &                 getBuf(void);
    std::vector<char>                   getBody(void) const;
    std::vector<char> &                 getBody(void);
    std::map<std::string, std::string>  getHeader() const;
    std::string &                       getHeaderField(const std::string & key);
    size_t                              getHeaderFieldAsNumber(const std::string & key);

private:
    request_states                      _request_state;
    t_start_line                        _start_line;
    std::map<std::string, std::string>  _header;
    std::vector<char>                   _buf;
    std::vector<char>                   _body;

    chunk_states                        _chunk_state;
    size_t                              _chunk_size;

    bool                                _bufContains(std::string str);
    std::vector<char>::iterator         _bufFind(std::string str);
    bool                                _parseHeader();
    bool                                _parseChunk();
    void                                _setBody();
    void                                _deleteHeaderInBuf();
    void                                _printBuf(std::string msg);
};

std::ostream & operator<<(std::ostream & o, Request const & other);

#endif
