#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <netinet/in.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <map>
# include <vector>
# include <string>
# include <iostream>
# include "Client.hpp"
# include "Setting.hpp"

class WebServer {
 public:
    explicit WebServer(const Setting & config);
    ~WebServer(void);
    WebServer(const WebServer & other);
    WebServer & operator=(const WebServer & other);

    void                    initServer();

    int                     getLs() const;
    Setting                 getConfig() const;
    std::vector<Client>&    getClients();
    sockaddr_in             getAddr() const;
    void                    appendClient(Client client);

 private:
    int                     _ls;
    Setting                 _config;
    std::vector<Client>     _clients;
    struct sockaddr_in      _addr;

    WebServer(void);
};

#endif
