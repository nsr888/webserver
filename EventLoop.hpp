#ifndef EVENTLOOP_HPP
# define EVENTLOOP_HPP
# include <netinet/in.h>
# include <string>
# include <vector>
# include <iostream>
# include "WebServer.hpp"

class EventLoop {
 public:
    EventLoop(void);
    explicit EventLoop(std::vector<Setting> config);
    ~EventLoop(void);
    EventLoop(const EventLoop & other);
    EventLoop & operator=(const EventLoop & other);

    void                    initServers();
    void                    runLoop();
    void                    shutdown();
    void                    appendWebServer(WebServer server);

 private:
    std::vector<WebServer>  _webservers;
    fd_set                  _readfds, _writefds;
    int                     _max_fd;
    int                     _cnt;

    int                     _getMaxLs();
    void                    _prepairSelect();
    void                    _acceptConnection();
    void                    _processClients();
};

#endif
