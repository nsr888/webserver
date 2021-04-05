#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include "Client.hpp"

int main()
{
    int ls;
    struct sockaddr_in addr;
    int port = 7777;
    std::vector<Client> clients;
    clients.clear();

    /* init server */
    /* \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
    ls = socket(AF_INET, SOCK_STREAM, 0);
    if (ls < 0) {
        std::cerr << "socket: " << strerror(errno) << std::endl;
        exit(1);
    }

    fcntl(ls, F_SETFL, O_NONBLOCK);
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(ls, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        std::cerr << "bind: " << strerror(errno) << std::endl;
        exit(2);
    }

    listen(ls, 5);

    /* webserver main cycle */
    /* \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
    /* http://www.stolyarov.info/books/programming_intro/vol3 (page 227) */
    /* https://rsdn.org/article/unix/sockets.xml */
    for (;;) {
        fd_set readfds, writefds;
        int max_fd = ls;
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_SET(ls, &readfds);
        for ( std::vector<Client>::iterator it = clients.begin();
                it != clients.end();) {
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

        /* ________ main cycle wait here on select _____ */
        int res = select(max_fd + 1, &readfds, &writefds, NULL, &timeout);

        if (res < 1) {
            if (errno != EINTR) {
                std::cerr << "select error: " << strerror(errno) << std::endl;
                exit(3);
            } else {
                std::cerr << "select signal: " << strerror(errno) << std::endl;
                exit(3);
            }
            continue;
        }
        if (res == 0) {
            std::cerr << "select timeout: " << strerror(errno) << std::endl;
            exit(3);
            /* continue; */
        }
        if (FD_ISSET(ls, &readfds)) {
            int sd;
            socklen_t len = sizeof(addr);
            sd = accept(ls, (struct sockaddr*) &addr, &len);
            if (sd == -1) {
                std::cerr << "accept: " << strerror(errno) << std::endl;
                exit(3);
            }
            fcntl(sd, F_SETFL, O_NONBLOCK);
            clients.push_back(Client(sd));
        }
        for (std::vector<Client>::iterator it = clients.begin();
                it != clients.end(); ) {
            if (FD_ISSET(it->getFd(), &readfds)) {
                it->readRequest();
                if (it->getState() == st_close_connection) {
                    it->closeConnection();
                    it = clients.erase(it);
                    continue;
                }
            }
            if (FD_ISSET(it->getFd(), &writefds)) {
                it->sendResponse();
            }
            ++it;
        }
    }
}
