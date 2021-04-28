#include "Client.hpp"
#include <exception>
#include <ostream>

const std::string yel("\033[0;33m");
const std::string red("\033[0;31m");
const std::string grn("\033[0;32m");
const std::string blu("\033[0;34m");
const std::string gra("\033[0;30m");
const std::string res("\033[0m");

Client::Client(const Client & other) { *this = other; }

Client::Client(int fd)
    : _client_state(st_read_request)
    , _fd(fd)
    , _buf()
    , _response()
    , _request()
{ }

Client::Client(void) { }

Client::~Client(void) { }

Client & Client::operator=(const Client & other) {
    _client_state = other.getState();
    _fd = other.getFd();
    _buf = other._buf;
    _response = other._response;
    _request = other._request;
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
        /* std::cout << "\nheader parsed with method: " << _request.getStartLine().method << std::endl; */
        if (!_request.isHeaderValid())
        {
            _client_state = st_generate_response; 
            return;
        }
        if (_request.getStartLine().method == "GET")
        {
            /* std::cout << "\nGET header parsed\n"; */
            _client_state = st_generate_response; 
        }
        else if (_request.getStartLine().method == "POST" && 
                _request.isBodyParsed())
        {
            /* std::cout << "\nPOST header and body parsed\n"; */
            _client_state = st_generate_response; 
        }
    }
    else if (_request.isBodyParsed())
    {
        /* std::cout << "\nbody parsed\n"; */
        _client_state = st_generate_response; 
    }
}

void Client::closeConnection() {
    close(_fd);
    std::cout << "Connection " << _fd << " closed" << std::endl;
}

void Client::sendResponse() {
    size_t bytes_sent = send(_fd, &_response[0], _response.size(), 0);
    if (bytes_sent == _response.size())
    {
        std::cout << "Full response of ";
        std::cout << bytes_sent << " bytes was sent" << std::endl;
        _request.clear();
        _response.clear();
        _response_struct.clear();
        _client_state = st_read_request;
    }
    else
    {
        _response.erase(_response.begin(), _response.begin() + bytes_sent);
        std::cout << "Response sent only ";
        std::cout << bytes_sent << " bytes" << std::endl;
        _client_state = st_send_response;
    }
}

void Client::generateResponse() {
    std::cout << yel << "\n------ start parsed request -------\n" << res;
    std::cout << _request << std::endl;
    std::cout << yel << "------ end parsed request -------" << res << std::endl;
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
    std::cout << red << "\n------ start response (first 400 symbols) -------\n" << res;
    std::cout << std::string(_response.begin(), _response.end()).substr(0, 400) << std::endl;
    std::cout << red << "------ end response (first 400 symbols) -------\n" << res;
}
