#include "Client.hpp"

Client::Client(const Client & other) { *this = other; }

Client::Client(int fd) : _state(st_read_request), _fd(fd) { }

Client::Client(void) { }

Client::~Client(void) { }

Client & Client::operator=(const Client & other) {
    this->_fd = other.getFd();
    this->_state = other.getState();
    return *this;
}

states Client::getState(void) const { return _state; }

int  Client::getFd() const { return _fd; }

void Client::readRequest() {
    int buf_size = 128;
    char buf[buf_size];
    int bytes_read = recv(this->_fd, buf, buf_size - 1, 0);
    std::cout << "bytes_read: " << bytes_read << std::endl;
    if (bytes_read <= 0) {
        this->_state = st_close_connection;
    } else {
        buf[bytes_read] = '\0';
        _request = _request + std::string(buf);
        std::size_t found = _request.find("\r\n\r\n");
        if (found != std::string::npos) {
            std::cout << "full request received" << std::endl;
            this->_state = st_generate_response;
        }
    }
}

void Client::closeConnection() {
    std::cout << "close connection" << _fd << ", 0 bytes readed\n";
    close(this->_fd);
}

void Client::sendResponse() {
    size_t bytes_sent = send(_fd, _response.data(), _response.length(), 0);
    if (bytes_sent == _response.length()) {
        this->_request = "";
        this->_state = st_read_request;
    } else {
        std::cout << "full response not sent, it must be chunked" << std::endl;
        this->_request = "";
        this->_state = st_read_request;
    }
}

void Client::generateResponse() {
    std::cout << "\n------ start request -------\n";
    std::cout << _request << std::endl;
    std::cout << "\n------ end request -------" << std::endl;
    std::size_t found = _request.find("GET /1 HTTP/1.1");
    if (found != std::string::npos)
        _response = "HTTP/1.1 200 OK\n"
            "Content-Type: text/plain\nContent-Length: 11\n\nArgument 1!";
    else
        _response = "HTTP/1.1 200 OK\n"
            "Content-Type: text/plain\nContent-Length: 12\n\nHello world!";
    this->_state = st_send_response;
}
