#include "EventLoop.hpp"
#include "utils.hpp"
#include <iostream>

EventLoop::EventLoop(const EventLoop & other) { 
    *this = other;
}

EventLoop::EventLoop(std::vector<Setting> config_vector)
{
    for (std::vector<Setting>::iterator config = config_vector.begin();
            config != config_vector.end(); ++config)
    {
        this->appendWebServer(WebServer(*config));
    }
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
    for (std::vector<WebServer>::iterator server = _webservers.begin();
         server != _webservers.end(); ++server)
    {
        if (server->getLs() > ls)
            ls = server->getLs();
    }
    return ls;
}

void EventLoop::initServers() {
    for (std::vector<WebServer>::iterator server = _webservers.begin();
         server != _webservers.end(); ++server)
    {
        server->initServer();
    }
}

void EventLoop::_prepairSelect() {
    for (std::vector<WebServer>::iterator server = _webservers.begin();
         server != _webservers.end(); ++server)
    {
        utils::log(server->getConfig(), __FILE__, __func__);
        FD_SET(server->getLs(), &_readfds);
        std::vector<Client>::iterator client = server->getClients().begin();
        while (client != server->getClients().end())
        {
            enum client_states state = client->getState();
            if (state == st_read_request || state == st_close_connection)
                FD_SET(client->getFd(), &_readfds);
            if (state == st_send_response)
                FD_SET(client->getFd(), &_writefds);
            if (client->getFd() > this->_max_fd)
                this->_max_fd = client->getFd();
            ++client;
        }
    }
}

void EventLoop::_acceptConnection() {
    std::vector<WebServer>::iterator server = _webservers.begin();
    while (server != _webservers.end())
    {
        int ls = server->getLs();
        sockaddr_in addr = server->getAddr();
        if (FD_ISSET(ls, &_readfds))
        {
            utils::log(server->getConfig(), __FILE__, __func__);
            socklen_t len = sizeof(addr);
            int fd;
            fd = accept(ls, (struct sockaddr*) &addr, &len);
            if (fd == -1)
                utils::e_throw("accept", __FILE__, __LINE__);
            if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
                utils::e_throw("accept fcntl", __FILE__, __LINE__);
            utils::log(server->getConfig(), __FILE__, "Append client #", fd);
            server->appendClient(Client(fd, &server->getConfig()));
        }
        ++server;
    }
}

void EventLoop::_processClients() {
    for (std::vector<WebServer>::iterator server = _webservers.begin();
         server != _webservers.end(); ++server)
    {
        Setting &config = server->getConfig();
        utils::log(config, __FILE__, __func__);
        std::vector<Client>::iterator client = server->getClients().begin();
        while (client != server->getClients().end())
        {
            if (FD_ISSET(client->getFd(), &_readfds))
            {
                utils::log(config, __FILE__, "readRequest");
                client->readRequest();
                if (client->getState() == st_generate_response)
                {
                    utils::log(config, __FILE__, "generateResponse");
                    client->generateResponse();
                }
                if (client->getState() == st_close_connection)
                {
                    utils::log(config, __FILE__, "Connection closed #",
                               client->getFd());
                    client->closeConnection();
                    FD_CLR(client->getFd(), &_readfds);
                    client = server->getClients().erase(client);
                    continue;
                }
            }
            if (FD_ISSET(client->getFd(), &_writefds))
            {
                utils::log(config, __FILE__, "sendResponse");
                client->sendResponse();
                _cnt = _cnt + 1;
            }
            ++client;
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
        std::cout << "\033[0;32m";
        std::cout << utils::get_current_time_fmt() << " ";
        std::cout << "Speed: " << "\033[0m";
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

        /* ________ wait point of server _____ */
        int res = select(_max_fd + 1, &_readfds, &_writefds, NULL, &timeout);
        if (res == 0)
        {
            shutdown();
            std::cout << "\033[0;32m";
            std::cout << utils::get_current_time_fmt() << " ";
            std::cout << "Shutdown by timeout" << "\033[0m" << std::endl;
            exit(EXIT_SUCCESS);
        }
        if (res < 1)
        {
            if (errno != EINTR)
                utils::e_throw("select error", __FILE__, __LINE__);
            else
                utils::e_throw("select signal", __FILE__, __LINE__);
        }

        this->_acceptConnection();
        this->_processClients();
    }
}

void EventLoop::shutdown() {
    for (std::vector<WebServer>::iterator server = _webservers.begin();
         server != _webservers.end(); ++server)
    {
        if (close(server->getLs()) < 0)
            utils::e_throw("listen socket close", __FILE__, __LINE__);
        std::vector<Client>::iterator client = server->getClients().begin();
        while (client != server->getClients().end())
        {
            client->closeConnection();
            client = server->getClients().erase(client);
            if (client == server->getClients().end())
                break;
            ++client;
        }
    }
}
