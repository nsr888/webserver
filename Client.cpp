#include "Client.hpp"
#include "utils.hpp"
#include <exception>
#include <iostream>
#include <ostream>

Client::Client(const Client & other) { *this = other; }

Client::Client(int fd, Setting * config)
    : _client_state(st_read_request)
    , _fd(fd)
    , _buf()
    , _response()
    , _request()
    , _response_struct(config)
    , _config(config)
    , _last_request_target()
{ }

Client::Client(void) { }

Client::~Client(void) { }

Client & Client::operator=(const Client & other) {
    _client_state = other.getState();
    _fd = other.getFd();
    _buf = other._buf;
    _response = other._response;
    _request = other._request;
    _response_struct = other._response_struct;
    _config = other._config;
    _last_request_target = other._last_request_target;
    return *this;
}

client_states Client::getState(void) const { return _client_state; }

int  Client::getFd() const { return _fd; }

void Client::readRequest() {
    std::vector<char> buf_read(1024);
    int bytes_read = recv(_fd, &buf_read[0], buf_read.size(), 0);
    if (bytes_read <= 0)
    {
        _client_state = st_close_connection;
        return;
    }
    if (_client_state == st_close_connection)
    {
        utils::log(*_config, __FILE__, "Can't close _fd");
        return;
    }
    buf_read.resize(bytes_read);
    _request.push_back(buf_read);
    if (_request.isHeaderParsed())
    {
        std::string method = _request.getStartLine().method;
        utils::log(*_config, __FILE__, "Header parsed with method " + method);
        if (!_request.isHeaderValid())
        {
            _client_state = st_generate_response; 
            return;
        }
        if (method == "GET" || method == "HEAD")
        {
            utils::log(*_config, __FILE__, "GET or HEAD header parsed");
            _client_state = st_generate_response; 
        }
        else if ((method == "POST" || method == "PUT") && _request.isBodyParsed())
        {
            utils::log(*_config, __FILE__, "POST or PUT header and body parsed");
            _client_state = st_generate_response; 
        }
    }
    else if (_request.isBodyParsed())
    {
        utils::log(*_config, __FILE__, "Body parsed");
        _client_state = st_generate_response; 
    }
}

void Client::closeConnection()
{
    utils::log(*_config, __FILE__, "Connection closed");
    close(_fd);
}

void Client::sendResponse() {
    int bytes_sent = send(_fd, &_response[0], _response.size(), 0);
    utils::log(*_config, __FILE__, "bytes_sent: ", bytes_sent);
    if (bytes_sent < 0)
    {
        utils::log(*_config, __FILE__, "Sent error");
        shutdown(_fd, SHUT_WR);
        _client_state = st_close_connection;
        return ;
    }
    if (static_cast<size_t>(bytes_sent) == _response.size())
    {
        utils::log(*_config, __FILE__, "Full response sent, bytes: ", bytes_sent);
        std::map<std::string, std::string> headers = _request.getHeader();
        if (headers.find("Connection") != headers.end() && 
                headers["Connection"] == "close")
        {
            shutdown(_fd, SHUT_WR);
            _client_state = st_close_connection;
            utils::ft_usleep(3);
        }
        else
            _client_state = st_read_request;
        /* clear, because we can get new request on same connection */
        _request.clear();
    }
    else
    {
        _response.erase(_response.begin(), _response.begin() + bytes_sent);
        utils::log(*_config, __FILE__, "Partial response sent, bytes: ", bytes_sent);
        _client_state = st_send_response;
    }
}

void Client::generateResponse() {
    if (_config->getDebugLevel() > 0)
    {
        std::cout << utils::YEL << "\n------ Request (parsed) Start (body only 400 symbols) -------\n" << utils::RES;
        std::cout << _request << std::endl;
        std::cout << utils::YEL << "------ Request (parsed) End (body only 400 symbols) -------" << utils::RES << std::endl;
    }
    if (_request.getStartLine().method != "GET"
            || _request.getStartLine().request_target != _last_request_target
            || _request.getHeaderField("Cache-control") == "no-cache")
    {
        std::vector<char>().swap (_response);
        _last_request_target = _request.getStartLine().request_target;
        _response_struct.generateResponseMsg(_request);
        _response = _response_struct.getBuf();
        _response_struct.clear();
        _request.buf_clear();
    }
    _client_state = st_send_response;
    if (_config->getDebugLevel() > 0)
    {
        std::cout << utils::RED << "\n------ Response Start (first 400 symbols) -------\n" << utils::RES;
        std::cout << std::string(_response.begin(), _response.end()).substr(0, 400) << std::endl;
        std::cout << utils::RED << "------ Response End (first 400 symbols) -------\n" << utils::RES;
    }
}
