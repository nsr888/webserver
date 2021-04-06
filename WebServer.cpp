#include "WebServer.hpp"

WebServer::WebServer(const WebServer & other) { *this = other; }

WebServer::WebServer(void) { }

WebServer::WebServer(const Settings & config)
    : _ls()
    , _config(config)
    , _clients()
    , _addr() { }

WebServer::~WebServer(void) { }

WebServer & WebServer::operator=(const WebServer & other) {
    this->_config = other.getConfig();
    return *this;
}

Settings WebServer::getConfig() const { return this->_config; }

void WebServer::_initServer()
{
    _ls = socket(AF_INET, SOCK_STREAM, 0);
    if (_ls < 0)
    {
        std::cerr << "socket: " << strerror(errno) << std::endl;
        exit(1);
    }
    fcntl(_ls, F_SETFL, O_NONBLOCK);
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(_config.port);
    _addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(_ls, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
    {
        std::cerr << "bind: " << strerror(errno) << std::endl;
        exit(2);
    }
    listen(_ls, 5);
}

/* http://www.stolyarov.info/books/programming_intro/vol3 (page 227) */
/* https://rsdn.org/article/unix/sockets.xml */
void WebServer::runServer()
{

    this->_initServer();

    for (;;) {
        fd_set readfds, writefds;
        int max_fd = _ls;
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_SET(_ls, &readfds);
        for ( std::vector<Client>::iterator it = _clients.begin();
                it != _clients.end();)
        {
            if (it->getState() == st_read_request)
                FD_SET(it->getFd(), &readfds);
            if (it->getState() == st_send_response)
                FD_SET(it->getFd(), &writefds);
            if (it->getFd() > max_fd)
                max_fd = it->getFd();
            ++it;
        }

        timeval timeout;
        timeout.tv_sec = 15;
        timeout.tv_usec = 0;

        /* ________ main cycle wait here _____ */
        int res = select(max_fd + 1, &readfds, &writefds, NULL, &timeout);

        if (res == 0)
        {
            std::cerr << "select timeout: " << strerror(errno) << std::endl;
            exit(3);
        }
        if (res < 1)
        {
            if (errno != EINTR)
            {
                std::cerr << "select error: " << strerror(errno) << std::endl;
                exit(3);
            }
            else
            {
                std::cerr << "select signal: " << strerror(errno) << std::endl;
                exit(3);
            }
        }
        if (FD_ISSET(_ls, &readfds))
        {
            int sd;
            socklen_t len = sizeof(_addr);
            sd = accept(_ls, (struct sockaddr*) &_addr, &len);
            if (sd == -1)
            {
                std::cerr << "accept: " << strerror(errno) << std::endl;
                exit(3);
            }
            fcntl(sd, F_SETFL, O_NONBLOCK);
            _clients.push_back(Client(sd));
        }
        for (std::vector<Client>::iterator it = _clients.begin();
                it != _clients.end(); )
        {
            if (FD_ISSET(it->getFd(), &readfds))
            {
                it->readRequest();
                if (it->getState() == st_generate_response)
                    it->generateResponse();
                if (it->getState() == st_send_response)
                    it->sendResponse();
                if (it->getState() == st_close_connection)
                {
                    it->closeConnection();
                    it = _clients.erase(it);
                    continue;
                }
            }
            if (FD_ISSET(it->getFd(), &writefds))
                it->sendResponse();
            ++it;
        }
    }
}
