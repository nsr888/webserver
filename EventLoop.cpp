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

EventLoop::EventLoop(void) {}

EventLoop::~EventLoop(void) {} 

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
        if (it_server->getConfig().getDebugLevel() == -9)
            utils::log("EventLoop.cpp", "prepairSelect");
        FD_SET(it_server->getLs(), &_readfds);
        /* utils::log("EventLoop.cpp", "ls: " */ 
        /*         + utils::to_string(it_server->getLs())); */
        /* utils::log("EventLoop.cpp", "getClients size: " */ 
        /*         + utils::to_string(it_server->getClients().size())); */
        std::vector<Client>::iterator it = it_server->getClients().begin();
        while (it != it_server->getClients().end())
        {
            if (it->getState() == st_read_request 
                    || it->getState() == st_close_connection)
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
            if (it->getConfig().getDebugLevel() == -9)
                utils::log("EventLoop.cpp", "acceptConnection");
            int sd;
            socklen_t len = sizeof(addr);
            sd = accept(ls, (struct sockaddr*) &addr, &len);
            if (sd == -1)
                throw std::runtime_error(std::string("accept: ") + strerror(errno));
            if (fcntl(sd, F_SETFL, O_NONBLOCK) < 0)
                throw std::runtime_error(std::string("fcntl: ") + strerror(errno));
            if (it->getConfig().getDebugLevel() == -9)
                utils::log("EventLoop.cpp", 
                        "Append client with fd: " + utils::to_string(sd));
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
        if (it_server->getConfig().getDebugLevel() == -9)
            utils::log("EventLoop.cpp", "processClients");
        std::vector<Client>::iterator it = it_server->getClients().begin();
        while (it != it_server->getClients().end())
        {
            if (FD_ISSET(it->getFd(), &_readfds))
            {
                if (it_server->getConfig().getDebugLevel() == -9)
                    utils::log("EventLoop.cpp", "readRequest");
                it->readRequest();
                if (it->getState() == st_generate_response)
                {
                    if (it_server->getConfig().getDebugLevel() == -9)
                        utils::log("EventLoop.cpp", "generateResponse");
                    it->generateResponse();
                }
                /* if (it->getState() == st_send_response) */
                /* { */
                /*     if (it_server->getConfig().getDebugLevel() == -9) */
                /*         utils::log("EventLoop.cpp", "sendResponse"); */
                /*     it->sendResponse(); */
                /*     _cnt = _cnt + 1; */
                /*     /1* std::cout << "_cnt: " << _cnt << std::endl; *1/ */
                /* } */
                if (it->getState() == st_close_connection)
                {
                    if (it_server->getConfig().getDebugLevel() == -9)
                        utils::log("EventLoop.cpp", 
                                "Connection " + 
                                utils::to_string(it->getFd()) + " closed");
                    it->closeConnection();
                    FD_CLR(it->getFd(), &_readfds);
                    it = it_server->getClients().erase(it);
                    continue;
                }
            }
            if (FD_ISSET(it->getFd(), &_writefds))
            {
                if (it_server->getConfig().getDebugLevel() == -9)
                    utils::log("EventLoop.cpp", "sendResponse");
                it->sendResponse();
                _cnt = _cnt + 1;
                /* if (it->getState() == st_close_connection) */
                /* { */
                /*     if (it_server->getConfig().getDebugLevel() == -9) */
                /*         utils::log("EventLoop.cpp", */ 
                /*                 "Connection " + */ 
                /*                 utils::to_string(it->getFd()) + " closed"); */
                /*     it->closeConnection(); */
                /*     FD_CLR(it->getFd(), &_writefds); */
                /*     it = it_server->getClients().erase(it); */
                /*     continue; */
                /* } */
            }
            ++it;
        }
    }
}

void show_speed(int* _last_count, int* _last_select_time, int _cnt)
{
    int time = utils::get_current_time_in_ms();
    size_t time_diff = time - *_last_select_time;
    size_t cnt_diff = _cnt - *_last_count;
    if (time_diff > 5000 && _cnt > 10)
    {
        std::cout << "\033[0;32m" << "Speed: " << "\033[0m";
        std::cout << cnt_diff * 1000 / time_diff << " trans/sec" << std::endl;
        *_last_count = _cnt;
        *_last_select_time = time;
    }
}

void EventLoop::runLoop() {
    /* http://www.stolyarov.info/books/programming_intro/vol3 (page 227) */
    /* https://rsdn.org/article/unix/sockets.xml */
    int                     _last_count = 0;
    int                     _last_select_time;
    _last_select_time = utils::get_current_time_in_ms();
    for (;;) {
        _max_fd = _getMaxLs();
        FD_ZERO(&_readfds);
        FD_ZERO(&_writefds);

        this->_prepairSelect();

        timeval timeout;
        timeout.tv_sec = 30;
        timeout.tv_usec = 0;

        show_speed(&_last_count, &_last_select_time, _cnt);

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
