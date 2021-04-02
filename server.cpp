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
    int buf_size = 3000;
    char buf[buf_size];
    int bytes_read;
    const std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    int port = 7777;
    std::vector<Client> clients;
    clients.clear();

    /* init server */
    /* \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
    ls = socket(AF_INET, SOCK_STREAM, 0);
    if(ls < 0)
    {
        perror("socket");
        exit(1);
    }
    
    fcntl(ls, F_SETFL, O_NONBLOCK);
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(ls, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    listen(ls, 5);

    /* webserver main cycle */
    /* \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
    /* http://www.stolyarov.info/books/programming_intro/vol3 (page 227) */
    /* https://rsdn.org/article/unix/sockets.xml */
    for(;;) {
        std::cout << "start" << std::endl;
        fd_set readfds, writefds;
        int max_fd = ls;
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_SET(ls, &readfds);
        for(std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
        {
            if (it->WantToRead())
                FD_SET(it->getFd(), &readfds);
            if (it->WantToWrite())
                FD_SET(it->getFd(), &writefds);
            if (it->getFd() > max_fd)
                max_fd = it->getFd();
        }

        timeval timeout;
        timeout.tv_sec = 15;
        timeout.tv_usec = 0;

        /* int res = select(max_fd + 1, &readfds, &writefds, NULL, &timeout); */
        std::cout << "select" << std::endl;
        int res = select(max_fd + 1, &readfds, &writefds, NULL, NULL);
        if (res < 1) {
            if (errno != EINTR) {
                perror("select error");
                exit(3);
            } else {
                perror("select signal");
                exit(3);
            }
            continue;
        }
        if (res == 0) {
            perror("select timeout");
            exit(3);
            /* continue; */
        }
        if (FD_ISSET(ls, &readfds)) {
            int sd;
            socklen_t len = sizeof(addr);
            std::cout << "accept" << std::endl;
            sd = accept(ls, (struct sockaddr*) &addr, &len);
            if(sd == -1)
            {
                perror("accept");
                exit(3);
            }
            fcntl(sd, F_SETFL, O_NONBLOCK);
            /* std::cout << "push_back" << std::endl; */
            clients.push_back(Client(sd));
        }
        std::cout << "second loop" << std::endl;
        for(std::vector<Client>::iterator it = clients.begin(); it != clients.end(); )
        {
            std::cout << "fd: " << it->getFd() << std::endl;
            if(FD_ISSET(it->getFd(), &readfds))
            {
                std::cout << "ready to read" << std::endl;
                // Ready to read from client
                bytes_read = recv(it->getFd(), buf, buf_size - 1, 0);
                if(bytes_read <= 0)
                {
                    // Connection closed, delete client fd
                    std::cout << "close connection, 0 bytes readed\n";
                    close(it->getFd());
                    it = clients.erase(it);
                    continue;
                }
                if (bytes_read == buf_size - 1) {
                }
                /* std::cout << bytes_read << std::endl; */
                buf[bytes_read] = '\0';
                it->processRequest(buf);
                /* it->sendResponse(); */
            }
            /* std::cout << "check isset in writefds" << std::endl; */
            if(FD_ISSET(it->getFd(), &writefds))
            {
                std::cout << "ready to write" << std::endl;
                // Ready to write to client
                it->sendResponse();
            }
            ++it;
        }
    }
}
