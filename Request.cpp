#include "Request.hpp"

const std::string yel("\033[0;33m");
const std::string red("\033[0;31m");
const std::string grn("\033[0;32m");
const std::string blu("\033[0;34m");
const std::string gra("\033[0;30m");
const std::string res("\033[0m");

Request::Request(void)
    : _request_state(st_header_feed)
    , _start_line()
    , _header()
    , _buf()
{}

Request::Request(const Request & other) { *this = other; }

Request & Request::operator=(const Request & other)
{
    _request_state = other.getState();
    _start_line = other.getStartLine();
    _header = other.getHeader();
    _buf = other.getBuf();
	return *this;
}

Request::~Request(void) {}

bool Request::_parseHeader() {
    const char *get = "GET";
    std::vector<char>::iterator it = std::search(_buf.begin(), _buf.end(), get, get + std::strlen(get));
    if (it != _buf.end())
    {
        _start_line.method = "GET";
        _start_line.request_target = "/";
        _start_line.http_version = "HTTP/1.1";
        _header["Host"] = "localhost";
        _header["Accept"] = "*/*";
        _printBuf("print raw buf, before delete header");
        _deleteHeaderInBuf();
        _request_state = st_body_feed;
        return true;
    }
    const char *post = "POST";
    std::vector<char>::iterator it_post = std::search(_buf.begin(), _buf.end(), post, post + std::strlen(post));
    if (it_post != _buf.end())
    {
        _start_line.method = "POST";
        _start_line.request_target = "/echo/post/form";
        _start_line.http_version = "HTTP/1.1";
        _header["Content-type"] = "application/x-www-form-urlencoded";
        _header["Host"] = "localhost";
        _header["Content-Length"] = "23";
        _header["Accept"] = "text/plain";
        _printBuf("print raw buf, before delete header");
        _deleteHeaderInBuf();
        _request_state = st_body_feed;
        return true;
    }
    /* if header cant be parsed, then _deleteHeaderInBuf and set _request_state = st_header_feed */
    _deleteHeaderInBuf();
    _request_state = st_header_feed;
    return false;
}

request_states Request::getState() const {
    return _request_state;
}

std::map<std::string, std::string> Request::getHeader() const {
    return _header;
}

std::string & Request::getHeaderField(const std::string & k)
{
	return _header[k];
}

t_start_line Request::getStartLine(void) const
{
	return _start_line;
}

std::vector<char> & Request::getBuf(void) {
	return _buf;
}

std::vector<char> Request::getBuf(void) const {
	return _buf;
}

std::vector<char> & Request::getBody(void) {
	return _body;
}

std::vector<char> Request::getBody(void) const {
	return _body;
}

void    Request::_printBuf(std::string msg) {
    std::cout << yel << "\n------ start " << msg << "-------\n" << res;
    std::vector<char>::iterator itv = _buf.begin();
    while (itv != _buf.end())
    {
        std::cout << *itv;
        ++itv;
    }
    std::cout << yel << "\n----- end " << msg << " -------" << res << std::endl;
}

void    Request::_deleteHeaderInBuf() {
        const char *crlf = "\r\n\r\n";
        std::vector<char>::iterator it = std::search(_buf.begin(), _buf.end(), crlf, crlf + std::strlen(crlf));
        if (it != _buf.end())
        {
            it = it + 4;
            _buf.assign(it, _buf.end());
        }
}

bool    Request::isHeaderParsed()
{
    if (_request_state == st_header_feed)
    {
        const char *crlf = "\r\n\r\n";
        std::vector<char>::iterator it;
        it = std::search (_buf.begin(), _buf.end(),
                crlf, crlf + std::strlen(crlf));
        /* if found end of header in _buf */
        if (it != _buf.end())
            return(_parseHeader());
    }
    return false;
}

bool    Request::isBodyParsed()
{
    if (_request_state == st_body_feed)
    {
        size_t content_length = static_cast<size_t>(ft_atoi(_header["Content-Length"].c_str()));
        if (_buf.size() >= content_length)
        {
            std::cout << "body is ready, _buf.size():" << _buf.size() << ", content_length: " << content_length << std::endl;
            _printBuf("buf, when body is ready");
            _setBody();
            return true;
        }
        std::cout << "body is NOT ready, _buf.size():" << _buf.size() << ", content_length: " << content_length << std::endl;
    }
    return false;
}

void    Request::_setBody()
{
    size_t content_length = static_cast<size_t>(ft_atoi(_header["Content-Length"].c_str()));
    std::vector<char>::iterator it = _buf.begin();
    _body.assign(it, it + content_length);
    it = it + content_length;
    _buf.assign(it, _buf.end());
    _request_state = st_body_ready;
}

void    Request::push_back(std::vector<char> buf_read)
{
    _buf.insert(_buf.end(), buf_read.begin(), buf_read.end());
}

void Request::clear() {
    _request_state = st_header_feed;
    _start_line = t_start_line();
    _header.clear();
    _body.clear();
}

std::ostream & operator<<(std::ostream & o, Request const & request) {
    o << request.getStartLine().method << " ";
    o << request.getStartLine().request_target << " ";
    o << request.getStartLine().http_version << "\n";
    std::map<std::string, std::string>  m = request.getHeader();
    std::map<std::string, std::string>::iterator it = m.begin();
    while (it != m.end())
    {
        o << it->first << ": " << it->second << "\n";
        ++it;
    }
    o << "\r\n\r\n";
    std::vector<char> v = request.getBody();
    std::vector<char>::iterator itv = v.begin();
    while (itv != v.end())
    {
        o << *itv;
        ++itv;
    }
    o << std::endl;
    return o;
}
