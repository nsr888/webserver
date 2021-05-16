#include "EventLoop.hpp"
#include "utils.hpp"
#include <iostream>

EventLoop::EventLoop(const EventLoop & other) { 
    *this = other;
}

EventLoop::EventLoop(std::vector<Setting> config)
{
    for ( std::vector<Setting>::iterator it = config.begin();
            it != config.end();++it)
        this->appendWebServer(WebServer(*it));
}

EventLoop::EventLoop(void) { }

EventLoop::~EventLoop(void) { }

EventLoop & EventLoop::operator=(const EventLoop & other) {
    this->_webservers = other._webservers;
    return *this;
}

void EventLoop::appendWebServer(WebServer server) {
    this->_webservers.push_back(server);
}

int EventLoop::_getMaxLs() {
    int ls = -1;
    for (std::vector<WebServer>::iterator it = _webservers.begin();
            it != _webservers.end(); ++it)
    {
        if (it->getLs() > ls)
            ls = it->getLs();
    }
    return ls;
}

void EventLoop::initServers() {
    for ( std::vector<WebServer>::iterator it = _webservers.begin();
            it != _webservers.end(); ++it)
        it->initServer();
}

void EventLoop::_prepairSelect() {
    for (std::vector<WebServer>::iterator it_server = _webservers.begin();
            it_server != _webservers.end();++it_server)
    {
        if (it_server->getConfig().getDebugLevel() > 3)
        {
            std::cout << "prepairSelect" << std::endl;
        }

        FD_SET(it_server->getLs(), &_readfds);
        std::vector<Client>::iterator it = it_server->getClients().begin();
        while (it != it_server->getClients().end())
        {
            if (it->getState() == st_read_request)
                FD_SET(it->getFd(), &_readfds);
            if (it->getState() == st_send_response)
                FD_SET(it->getFd(), &_writefds);
            if (it->getFd() > this->_max_fd)
                this->_max_fd = it->getFd();
            ++it;
        }
    }
}

void EventLoop::_acceptConnection() {
    std::vector<WebServer>::iterator it = _webservers.begin();
    while (it != _webservers.end())
    {
        int ls = it->getLs();
        sockaddr_in addr = it->getAddr();
        if (FD_ISSET(ls, &_readfds))
        {
            if (it->getConfig().getDebugLevel() > 3)
            {
                std::cout << "acceptConnection" << std::endl;
            }
            int sd;
            socklen_t len = sizeof(addr);
            sd = accept(ls, (struct sockaddr*) &addr, &len);
            if (sd == -1)
                throw std::runtime_error(std::string("accept: ") + strerror(errno));
            fcntl(sd, F_SETFL, O_NONBLOCK);
            if (it->getConfig().getDebugLevel() > 3)
            {
                std::cout << "Append client fd " << sd << std::endl;
            }
            it->appendClient(Client(sd, &it->getConfig()));
            /* std::cout << "Clients size " << it->getClients().size() << std::endl; */
        }
        ++it;
    }
}

void EventLoop::_processClients() {
    for (std::vector<WebServer>::iterator it_server = _webservers.begin();
            it_server != _webservers.end();++it_server)
    {
        if (it_server->getConfig().getDebugLevel() > 3)
        {
            std::cout << "processClients" << std::endl;
        }
        std::vector<Client>::iterator it = it_server->getClients().begin();
        while (it != it_server->getClients().end())
        {
            if (FD_ISSET(it->getFd(), &_readfds))
            {
                if (it_server->getConfig().getDebugLevel() > 3)
                {
                    std::cout << "readRequest" << std::endl;
                }
                it->readRequest();
                if (it->getState() == st_generate_response)
                {
                    if (it_server->getConfig().getDebugLevel() > 3)
                    {
                        std::cout << "generateResponse" << std::endl;
                    }
                    it->generateResponse();
                }
                if (it->getState() == st_close_connection)
                {
                    it->closeConnection();
                    it = it_server->getClients().erase(it);
                    continue;
                }
            }
            if (FD_ISSET(it->getFd(), &_writefds))
            {
                if (it_server->getConfig().getDebugLevel() > 3)
                {
                    std::cout << "sendResponse at the end" << std::endl;
                }
                it->sendResponse();
                if (it->getState() == st_close_connection)
                {
                    it->closeConnection();
                    it = it_server->getClients().erase(it);
                    continue;
                }
            }
            ++it;
        }
    }
}

void EventLoop::runLoop() {
    /* http://www.stolyarov.info/books/programming_intro/vol3 (page 227) */
    /* https://rsdn.org/article/unix/sockets.xml */
    for (;;) {
        _max_fd = _getMaxLs();
        FD_ZERO(&_readfds);
        FD_ZERO(&_writefds);

        this->_prepairSelect();

        timeval timeout;
        timeout.tv_sec = 30;
        timeout.tv_usec = 0;

        /* ________ main cycle wait here _____ */
        int res = select(_max_fd + 1, &_readfds, &_writefds, NULL, &timeout);
        if (res == 0)
        {
            shutdown();
            std::cout << "\033[0;32m" << "Shutdown by timeout" << "\033[0m" << std::endl;
            exit(EXIT_SUCCESS);
        }
        if (res < 1)
        {
            if (errno != EINTR)
                throw std::runtime_error(
                        std::string("select error: ") + strerror(errno));
            else
                throw std::runtime_error(
                        std::string("select signal: ") + strerror(errno));
        }

        this->_acceptConnection();
        this->_processClients();
    }
}

void EventLoop::shutdown() {
    for (std::vector<WebServer>::iterator it_server = _webservers.begin();
            it_server != _webservers.end();++it_server)
    {
        if(close(it_server->getLs()) < 0)
        {
                throw std::runtime_error(
                        std::string("listen socket close: ") + strerror(errno));
        }
        std::vector<Client>::iterator it = it_server->getClients().begin();
        while (it != it_server->getClients().end())
        {
            it->closeConnection();
            it = it_server->getClients().erase(it);
            if (it == it_server->getClients().end())
                break;
            ++it;
        }
    }
}
