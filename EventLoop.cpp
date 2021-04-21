#include "EventLoop.hpp"
#include <iostream>

const std::string yel("\033[0;33m");
const std::string red("\033[0;31m");
const std::string grn("\033[0;32m");
const std::string blu("\033[0;34m");
const std::string gra("\033[0;30m");
const std::string res("\033[0m");

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
    /* collect fds from all webservers clients*/
    for (std::vector<WebServer>::iterator it_server = _webservers.begin();
            it_server != _webservers.end();++it_server)
    {
        FD_SET(it_server->getLs(), &_readfds);
        std::vector<Client>::iterator it = it_server->getClients().begin();
        /* int n = 0; */
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
            int sd;
            socklen_t len = sizeof(addr);
            sd = accept(ls, (struct sockaddr*) &addr, &len);
            if (sd == -1)
                throw std::runtime_error(std::string("accept: ") + strerror(errno));
            fcntl(sd, F_SETFL, O_NONBLOCK);
            it->appendClient(Client(sd));
        }
        ++it;
    }
}

void EventLoop::_processClients() {
    for (std::vector<WebServer>::iterator it_server = _webservers.begin();
            it_server != _webservers.end();++it_server)
    {
        std::vector<Client>::iterator it = it_server->getClients().begin();
        while (it != it_server->getClients().end())
        {
            if (FD_ISSET(it->getFd(), &_readfds))
            {
                it->readRequest();
                if (it->getState() == st_generate_response)
                {
                    it->generateResponse();
                }
                /* if (it->getState() == st_send_response) */
                /* { */
                /*     it->sendResponse(); */
                /* } */
                if (it->getState() == st_close_connection)
                {
                    it->closeConnection();
                    it = it_server->getClients().erase(it);
                    continue;
                }
            }
            if (FD_ISSET(it->getFd(), &_writefds))
                it->sendResponse();
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
            std::cout << grn << "Shutdown by timeout" << std::endl;
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
