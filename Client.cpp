#include "Client.hpp"

Client::Client(const Client & other)
{
    *this = other;
}

Client::Client(int fd)
    : _fd(fd)
    , _want_to_read(true)
    , _want_to_write(false)
{
}

Client::Client(void) { }

Client::~Client(void)
{
}

Client & Client::operator=(const Client & other)
{
    this->_fd = other._fd;
    this->_want_to_read = other._want_to_read;
    this->_want_to_write = other._want_to_write;
    return *this;
}

bool Client::WantToRead(void) const
{
	return _want_to_read;
}

bool Client::WantToWrite(void) const
{
	return _want_to_write;
}

int  Client::getFd() const
{
	return _fd;
}

void  Client::sendResponse()
{
    send(_fd, _response.data(), _response.length(), 0);
    this->_want_to_write = false;
}

void Client::processRequest(const char * request)
{
    std::string tmp(request);
    /* bool is_request_header_full = false; */
    /* if (tmp.find("\r\n\r\n") != std::string::npos) */
    /*     bool is_request_header_full = true; */
    std::cout << tmp << std::endl;
    std::size_t found = tmp.find("GET /1 HTTP/1.1");
    if (found != std::string::npos)
        _response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 11\n\nArgument 1!";
    else
        _response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    this->_want_to_write = true;
}

