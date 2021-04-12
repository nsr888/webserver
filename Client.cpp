#include "Client.hpp"

const std::string yel("\033[0;33m");
const std::string red("\033[0;31m");
const std::string grn("\033[0;32m");
const std::string blu("\033[0;34m");
const std::string gra("\033[0;30m");
const std::string res("\033[0m");

Client::Client(const Client & other) { *this = other; }

Client::Client(int fd) : _client_state(st_read_request), _fd(fd) { }

Client::Client(void) { }

Client::~Client(void) { }

Client & Client::operator=(const Client & other) {
    _fd = other.getFd();
    _client_state = other.getState();
    return *this;
}

client_states Client::getState(void) const { return _client_state; }

int  Client::getFd() const { return _fd; }

void Client::readRequest() {
    int buf_size = 2; // must be 2 or more
    char buf[buf_size];
    int bytes_read = recv(_fd, buf, buf_size - 1, 0);
    buf[bytes_read] = '\0';
    if (bytes_read <= 0)
    {
        _client_state = st_close_connection;
    }
    else
    {
        if (_request.getState() == st_header_feed ||
            _request.getState() == st_body_feed)
        {
            _request.appendBuf(buf);
        }
        if (_request.getState() == st_header_found)
        {
            _request.parseHeader();
        }
        if (_request.getState() == st_header_parsed)
        {
            if (_request.getStartLine().method == "GET")
            {
                std::cout << "\nGET header parsed" << std::endl;
                _client_state = st_generate_response; 
            }
            else if (_request.getStartLine().method == "POST")
            {
                std::cout << "\nPOST header parsed" << std::endl;
                _request.checkBodyFull();
            }
        }
        if (_request.getState() == st_body_ready)
            _client_state = st_generate_response; 
    }
}

void Client::closeConnection() {
    close(_fd);
    std::cout << "Connection " << _fd << " closed" << std::endl;
}

void Client::sendResponse() {
    size_t bytes_sent = send(_fd, _response.data(), _response.length(), 0);
    if (bytes_sent == _response.length())
    {
        std::cout << "Response was sent" << std::endl;
        _request.clear();
        _client_state = st_read_request;
    }
    else
    {
        std::cout << "Response must be chunked here" << std::endl;
        _client_state = st_read_request;
    }
}

void Client::generateResponse() {
    std::cout << yel << "\n------ start print parsed request -------\n" << res;
    std::cout << _request << std::endl;
    std::cout << yel << "\n------ end print parsed request -------" << res << std::endl;
    _response = "HTTP/1.1 200 OK\n"
        "Content-Type: text/plain\nContent-Length: 12\n\nHello world!";
    _client_state = st_send_response;
}
