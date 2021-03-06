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
    , _body()
    , _chunk_state(ch_header_feed)
    , _chunk_size()
{}

Request::Request(const Request & other) { *this = other; }

Request & Request::operator=(const Request & other)
{
    _request_state = other.getState();
    _start_line = other.getStartLine();
    _header = other.getHeader();
    _buf = other.getBuf();
    _body = other._body;
    _chunk_state = other._chunk_state;
    _chunk_size = other._chunk_size;
	return *this;
}

Request::~Request(void) {}

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

std::vector<char>    Request::deleteHeaderInBuf(std::vector<char> * buf) {
    const char *crlf = "\r\n\r\n";
    std::vector<char> deleted_headers;
    std::vector<char>::iterator it = std::search(buf->begin(), buf->end(), crlf, crlf + std::strlen(crlf));
    if (it != buf->end())
    {
        deleted_headers.insert(deleted_headers.end(), buf->begin(), it);
        buf->erase(buf->begin(), it + 4);
    }
    deleted_headers.push_back(0);
    return deleted_headers;
}

bool Request::_parseHeader() {
    
    std::vector<char>::iterator head = _buf.begin();
   	std::vector<char>::iterator tail = head;

    // Skip CRLF, it appear after POST request
	while (tail != _buf.end() && (*tail == '\r' || *tail == '\n'))
		++tail;
	head = tail;
	// Find type
	while (tail != _buf.end() && *tail != ' ')
		++tail;
	_start_line.method = std::string(head, tail);

	// Find path
	while (tail != _buf.end() && *tail == ' ')
		++tail;
	head = tail;
	while (tail != _buf.end() && *tail != ' ')
		++tail;
	_start_line.request_target = std::string(head, tail);

	// Find HTTP version
	while (tail != _buf.end() && *tail == ' ')
		++tail;
	head = tail;
	while (tail != _buf.end() && *tail != '\r')
		++tail;
	_start_line.http_version = std::string(head, tail);
	while (tail != _buf.end() && (*tail == '\r' || *tail == '\n'))
		++tail; // skip /r

	// All headers in map (key - value)
	head = tail;
	while (head != _buf.end() && *head != '\r')
	{
		while (tail != _buf.end() && *tail != '\r')
			++tail;
		std::vector<char>::iterator separator = head;
		while (separator != _buf.end() && separator != tail && *separator != ':')
			++separator;
		if (separator == tail)
			break;
		std::string key(head, separator);
   		std::vector<char>::iterator value = ++separator;
		while (value != tail && (*value == ' ' || *value == ':'))
			++value;
		_header[key] = std::string(value, tail);
		while (tail != _buf.end() && (*tail == '\r' || *tail == '\n'))
			++tail;
		head = tail;
    }
    /* _printBuf("print raw buf, before delete header"); */
    deleteHeaderInBuf(&this->_buf);
    _request_state = st_body_feed;
    return true;
}

request_states Request::getState() const {
    return _request_state;
}

std::map<std::string, std::string>  Request::getHeader() const {
    return _header;
}

std::string & Request::getHeaderField(const std::string & k)
{
	return _header[k];
}

size_t  Request::getHeaderFieldAsNumber(const std::string & key)
{
    return static_cast<size_t>(ft_atoi(_header[key].c_str()));
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

std::vector<char>::iterator
Request::bufFind(std::vector<char> * buf,std::string str) {
    /* find specified string in buf */
    const char *crlf = str.c_str();
    std::vector<char>::iterator it;
    it = std::search(buf->begin(), buf->end(), crlf, crlf + std::strlen(crlf));
    return it;
}

bool Request::bufContains(std::vector<char> * buf, std::string str) {
    /* indicates whether the _buf contains the specified string */
    if (bufFind(buf, str) != buf->end())
        return true;
    return false;
}

bool    Request::isHeaderParsed()
{
    /* indicates whether the _buf contains the end of header symbols */
    if (_request_state == st_header_feed && bufContains(&this->_buf, "\r\n\r\n"))
        return(_parseHeader());
    return false;
}

bool    Request::isBodyParsed()
{
    if (_request_state == st_body_feed)
    {
        if (_header.find("Content-Length") != _header.end())
        {
            size_t content_length = 
                static_cast<size_t>(ft_atoi(_header["Content-Length"].c_str()));
            if (_buf.size() >= content_length)
            {
                /* std::cout << "body is ready, _buf.size():" << _buf.size(); */
                /* std::cout << ", content_length: " << content_length << std::endl; */
                /* _printBuf("buf, when body is ready"); */
                _setBody();
                return true;
            }
            /* std::cout << "body is NOT ready, _buf.size():" << _buf.size(); */
            /* std::cout << ", content_length: " << content_length << std::endl; */
        }
        if (_header.find("Transfer-Encoding") != _header.end() &&
                _header["Transfer-Encoding"] == "chunked")
        {
            return(_parseChunk());
        }
    }
    return false;
}

bool    Request::_parseChunk()
{
    while (1)
    {
        if (_chunk_state == ch_header_feed)
        {
            /* utils::log("Request.cpp", "ch_header_feed"); */
            if (bufContains(&this->_buf, "\r\n"))
            {
                /* _printBuf("_buf, before chunk header parse"); */ 
                std::string str(_buf.begin(), bufFind(&this->_buf, "\r\n"));
                if (bufContains(&this->_buf, ";"))
                    str = std::string(_buf.begin(), bufFind(&this->_buf, ";"));
                if (str == "0" || str == "")
                    _chunk_size = 0;
                else
                    _chunk_size = std::strtol(str.c_str(), 0, 16);
                /* utils::log("Request.cpp", "chunk_size: " + utils::to_string(_chunk_size)); */
                _buf.erase(_buf.begin(), bufFind(&this->_buf, "\r\n") + 2);
                /* _buf.assign(bufFind(&this->_buf, "\r\n") + 2, _buf.end()); */
                _chunk_state = ch_body_feed;
            }
            else
            {
                return false;
            }
        }
        if (_chunk_state == ch_body_feed)
        {
            /* utils::log("Request.cpp", "ch_body_feed"); */
            if (_chunk_size == 0)
            {
                /* _printBuf("_buf, ch_body_feed, _chunk_size == 0"); */ 
                char *body_size = ft_itoa(_body.size());
                if (!body_size)
                    utils::e_throw("ft_itoa: ", __FILE__, __LINE__);
                _header["Content-Length"] = std::string(body_size);
                _header.erase("Transfer-Encoding");
                free(body_size);
                return true;
            }
            if (_buf.size() >= _chunk_size + 2)
            {
                /* _printBuf("_buf, ch_body_feed, _buf.size() >= _chunk_size + 2"); */ 
                _body.insert(_body.end(), _buf.begin(), _buf.begin() + _chunk_size);
                /* std::cout << "body is ready, _buf.size():" << _buf.size(); */
                /* std::cout << ", _chunk_size: " << _chunk_size << std::endl; */
                /* _printBuf("_buf, chunk body, before erase"); */
                _buf.erase(_buf.begin(), _buf.begin() + _chunk_size + 2);
                _chunk_state = ch_header_feed;
            }
            else
            {
                /* std::cout << "body is NOT ready, _buf.size():" << _buf.size(); */
                /* std::cout << ", _chunk_size: " << _chunk_size << std::endl; */
                return false;
            }
        }
    }
    return false;
}

void    Request::_setBody()
{
    size_t content_length = static_cast<size_t>(ft_atoi(_header["Content-Length"].c_str()));
    std::vector<char>::iterator it = _buf.begin();
    _body.assign(it, it + content_length);
    /* it = it + content_length; */
    _buf.erase(_buf.begin(), _buf.begin() + content_length);
    /* _buf.assign(it, _buf.end()); */
    _request_state = st_body_ready;
}

bool    Request::isHeaderContains(std::string str)
{
    if (_header.find(str) != _header.end())
        return true;
    return false;
}
void    Request::push_back(std::vector<char> buf_read)
{
    _buf.insert(_buf.end(), buf_read.begin(), buf_read.end());
}

void    Request::setChunkState(chunk_states chunk_state)
{
    _chunk_state = chunk_state;
}

void Request::buf_clear() {
    /* Clear memory used by vectors, using clear() method insufficient */
    /* https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Clear-and-minimize */
    std::vector<char>().swap(_buf);
    std::vector<char>().swap(_body);
}

void Request::clear() {
    _request_state = st_header_feed;
    _start_line = t_start_line();
    _header.clear();
    buf_clear();
    _chunk_state = ch_header_feed;
    _chunk_size = 0;
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
    o << "\n";
    std::vector<char> v = request.getBody();
    std::vector<char>::iterator itv = v.begin();
    o << std::string(itv, v.end()).substr(0, 400) << std::endl;
    /* while (itv != v.end()) */
    /* { */
    /*     o << *itv; */
    /*     ++itv; */
    /* } */
    o << std::endl;
    return o;
}

bool    Request::isHeaderValid() const {
    if (!isMethodValid() || !isHttpVersionValid() || !isRequestTargetValid())
        return false;
    return true;
}

bool    Request::isMethodValid() const {
    const char *methods[] = {
        "GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE"
    };
    std::vector<std::string> allowed_methods(methods, methods + 8);
    if (!utils::in_array(_start_line.method, allowed_methods))
        return false;
    return true;
}

bool    Request::isHttpVersionValid() const {
    if (_start_line.http_version != "HTTP/1.1")
        return false;
    return true;
}

bool    Request::isRequestTargetValid() const {
    if (_start_line.request_target.size() > 8000)
        return false;
    const std::string validChars = "!#$&'()*+,/:;=?@[]ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~%";
    for (size_t i = 0; i != _start_line.request_target.size(); ++i) 
    {
        if (validChars.find_first_of(_start_line.request_target[i]) == std::string::npos)
            return false;
    }
    return true;
}
