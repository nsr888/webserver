#include "Client.hpp"
#include "utils.hpp"
#include <exception>
#include <ostream>

Client::Client(const Client & other) { *this = other; }

Client::Client(int fd, Setting * config)
    : _client_state(st_read_request)
    , _fd(fd)
    , _buf()
    , _response()
    , _request()
    , _response_struct(config)
    , _time_last_response()
    , _config(config)
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
    _time_last_response = other._time_last_response;
    _config = other._config;
    return *this;
}

client_states Client::getState(void) const { return _client_state; }

int  Client::getFd() const { return _fd; }

void Client::readRequest() {
    std::vector<char> buf_read(3000);
    int bytes_read = recv(_fd, &buf_read[0], buf_read.size(), 0);
    if (bytes_read <= 0)
    {
        _client_state = st_close_connection;
        return;
    }
    buf_read.resize(bytes_read);
    _request.push_back(buf_read);
    if (_request.isHeaderParsed())
    {
        if (_config->getDebugLevel() > 2)
            utils::log("Client.cpp", "Header parsed with method " + _request.getStartLine().method);
        if (!_request.isHeaderValid())
        {
            _client_state = st_generate_response; 
            return;
        }
        if (_request.getStartLine().method == "GET" || 
                _request.getStartLine().method == "HEAD")
        {
            if (_config->getDebugLevel() > 2)
                utils::log("Client.cpp", "GET or HEAD header parsed");
            _client_state = st_generate_response; 
        }
        else if ((_request.getStartLine().method == "POST" || 
                   _request.getStartLine().method == "PUT") 
                    && _request.isBodyParsed())
        {
            if (_config->getDebugLevel() > 2)
                utils::log("Client.cpp", "POST or PUT header and body parsed");
            _client_state = st_generate_response; 
        }
    }
    else if (_request.isBodyParsed())
    {
        if (_config->getDebugLevel() > 2)
            utils::log("Client.cpp", "Body parsed");
        _client_state = st_generate_response; 
    }
}

void Client::closeConnection() {
    while (close(_fd) == EINTR)
        ;
}

void Client::sendResponse() {
    size_t bytes_sent = send(_fd, &_response[0], _response.size(), 0);
    if (bytes_sent == _response.size())
    {
        if (_config->getDebugLevel() > 3)
            utils::log("Client.cpp", "Full response was sent (" + utils::to_string(bytes_sent) + " bytes)");
        std::map<std::string, std::string> headers = _request.getHeader();
        if (headers.find("Connection") != headers.end() && 
                headers["Connection"] == "close")
        {
            _client_state = st_close_connection;
        }
        else
        {
            _client_state = st_read_request;
        }
        _request.clear();
        _response.clear();
        _response_struct.clear();
        /* _time_last_response = utils::get_current_time_in_ms(); */
        utils::ft_usleep(3);
    }
    else if (bytes_sent < 0)
    {
        _client_state = st_send_response;
    }
    else
    {
        _response.erase(_response.begin(), _response.begin() + bytes_sent);
        if (_config->getDebugLevel() > 3)
            utils::log("Client.cpp", "Partial response was sent (" + utils::to_string(bytes_sent) + " bytes)");
        _client_state = st_send_response;
    }
}

void Client::generateResponse() {
    if (_config->getDebugLevel() > 0)
    {
        std::cout << utils::YEL << "\n------ Request (parsed) Start -------\n" << utils::RES;
        std::cout << _request << std::endl;
        std::cout << utils::YEL << "------ Request (parsed) End -------" << utils::RES << std::endl;
    }
    /* if (_request.getStartLine().method == "POST" && */
    /*     _request.isHeaderContains("Content-Length") && */
    /*     _request.getHeaderFieldAsNumber("Content-Length") == 0) */
    /* { */
    /*     std::string body("Method Not Allowed"); */
    /*     std::string header = "HTTP/1.1 405 Method Not Allowed\n" */
    /*         "Content-Type: text/plain\nContent-Length: " + */
    /*         std::string(ft_itoa(body.size())) + "\n\n"; */
    /*     _response.assign(header.begin(), header.end()); */
    /*     _response.insert(_response.end(), body.begin(), body.end()); */
    /* } */
    /* else */
    /* { */
    /*     if (_request.getStartLine().request_target == "/files/test_large_file.html") */
    /*     { */
    /*         std::vector<char> tmp; */
    /*         tmp = utils::read_file("./files/test_large_file.html"); */
    /*         std::string body(tmp.begin(), tmp.end()); */
    /*         std::string header = "HTTP/1.1 200 OK\n" */
    /*             "Content-Type: text/html\nContent-Length: " + */
    /*             std::string(ft_itoa(body.size())) + "\n\n"; */
    /*         _response.assign(header.begin(), header.end()); */
    /*         _response.insert(_response.end(), body.begin(), body.end()); */
    /*     } */
    /*     else */
    /*     { */
    /*         std::string msg = "HTTP/1.1 200 OK\n" */
    /*             "Content-Type: text/plain\n" */
    /*             "Content-Length: 12\n\n" */
    /*             "Hello world!"; */
    /*         _response.assign(msg.begin(), msg.end()); */
    /*     } */
    /* } */
    _response_struct.generateResponseMsg(_request);
    _response.assign(_response_struct.getBuf().begin(), _response_struct.getBuf().end());

    _client_state = st_send_response;
    if (_config->getDebugLevel() > 0)
    {
        std::cout << utils::RED << "\n------ Response Start (first 400 symbols) -------\n" << utils::RES;
        std::cout << std::string(_response.begin(), _response.end()).substr(0, 400) << std::endl;
        std::cout << utils::RED << "------ Response End (first 400 symbols) -------\n" << utils::RES;
    }
}
