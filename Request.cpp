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

void Request::parseHeader() {
    const char *get = "GET";
    std::vector<char>::iterator it = std::search(_buf.begin(), _buf.end(), get, get + std::strlen(get));
    if (it != _buf.end())
    {
        _start_line.method = "GET";
        _start_line.request_target = "/";
        _start_line.http_version = "HTTP/1.1";
        _header["Host"] = "localhost";
        _header["Accept"] = "*/*";
        _delBufHeader();
        _request_state = st_header_parsed;
        return;
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
        _delBufHeader();
        _request_state = st_header_parsed;
        return;
    }
    /* if header cant be parsed, then _delBufHeader and set _request_state = st_header_feed */
    _delBufHeader();
    _request_state = st_header_feed;
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

void    Request::_debugPrint(std::string msg) {
    std::cout << yel << "\n------ start " << msg << "-------\n" << res;
    std::vector<char>::iterator itv = _buf.begin();
    while (itv != _buf.end())
    {
        std::cout << *itv;
        ++itv;
    }
    std::cout << yel << "\n----- end " << msg << " -------" << res << std::endl;
}

void    Request::_delBufHeader() {
        const char *crlf = "\r\n\r\n";
        std::vector<char>::iterator it = std::search(_buf.begin(), _buf.end(), crlf, crlf + std::strlen(crlf));
        /* _debugPrint("before delete header"); */
        if (it != _buf.end())
        {
            it = it + 4;
            _buf.assign(it, _buf.end());
            /* _debugPrint("after delete header"); */
        }
}

bool    Request::_findHeader()
{
    const char *crlf = "\r\n\r\n";
    std::vector<char>::iterator it;
    it = std::search (_buf.begin(), _buf.end(),
            crlf, crlf + std::strlen(crlf));
    if (it != _buf.end())
        return true;
    return false;
}

void    Request::checkBodyFull()
{
    int content_length = ft_atoi(_header["Content-Length"].c_str());
    int body_size = static_cast<int>(_buf.size());

    if (body_size >= content_length)
    {
        std::cout << "body is ready, _buf.size():" << body_size << ", content_length: " << content_length << std::endl;
        _debugPrint("buf, when body is ready");
        std::vector<char>::iterator it = _buf.begin();
        _body.assign(it, it + content_length);
        it = it + content_length;
        _buf.assign(it, _buf.end());
        _request_state = st_body_ready;
    }
    else
    {
        std::cout << "body is NOT ready, _buf.size():" << body_size << ", content_length: " << content_length << std::endl;
        _request_state = st_body_feed;
    }
}

void    Request::appendBuf(const char * buf)
{
    const char * buf_end = buf + std::strlen(buf);
    _buf.insert(_buf.end(), buf, buf_end);
    if (_request_state == st_header_feed && _findHeader())
    {
        _debugPrint("print raw request header");
        _request_state = st_header_found;
    }
    if (_request_state == st_body_feed)
        checkBodyFull();
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

std::string Request::getHeaderBuf() const {
    return _header_buf;
}
