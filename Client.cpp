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
    std::vector<char> buf_read(8);
    int bytes_read = recv(_fd, &buf_read[0], buf_read.size(), 0);
    buf_read.resize(bytes_read);
    if (bytes_read <= 0)
    {
        _client_state = st_close_connection;
        return;
    }
    _request.push_back(buf_read);
    if (_request.isHeaderParsed())
    {
        if (_request.getStartLine().method == "GET")
        {
            std::cout << "\nGET header parsed\n";
            _client_state = st_generate_response; 
        }
        else if (_request.getStartLine().method == "POST" && 
                _request.isBodyParsed())
        {
            std::cout << "\nPOST header and body parsed\n";
            _client_state = st_generate_response; 
        }
    }
    else if (_request.isBodyParsed())
    {
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
    _response_struct.setCode(200); /* Нужна функция для установки кода */
    _response_struct.generateResponse1(_request, 1, /* Здесь должен быть путь исходя из которого будет сформирован body */_request.getStartLine().request_target);
    _response = _response_struct.getBuf();
    std::cout << "Response message:" << _response << std::endl;
    _client_state = st_send_response;
}
