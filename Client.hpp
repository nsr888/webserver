#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <sys/socket.h>
# include <unistd.h>
# include <vector>
# include <string>
# include <iostream>
# include "Request.hpp"
# include "utils.hpp"
# include "Response.hpp"
# include "utils.hpp"

enum client_states {
    st_read_request,
    st_generate_response,
    st_send_response,
    st_close_connection,
    st_wait
};

class Client {
 public:
    explicit Client(int fd, Setting & config);
    ~Client(void);
    Client(const Client & other);
    Client & operator=(const Client & other);

    void                        readRequest();
    void                        generateResponse();
    void                        sendResponse();
    void                        closeConnection();

    client_states               getState(void) const;
    int                         getFd() const;

 private:
    client_states               _client_state;
    int                         _fd;
    std::vector<char>           _buf;
    std::vector<char>           _response;
    Request                     _request;
    Response                    _response_struct;
    size_t                      _time_last_response;
    Setting                     _config;

    Client(void);
    friend bool operator< (Client const& lhs, Client const& rhs) {
        return lhs.getFd() < rhs.getFd();
    }
};


#endif
