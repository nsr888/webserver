#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <sys/socket.h>
# include <unistd.h>
# include <vector>
# include <string>
# include <iostream>

enum states {
    st_read_request,
    st_generate_response,
    st_send_response,
    st_close_connection
};

class Client {
 public:
    explicit Client(int fd);
    ~Client(void);
    Client(const Client & other);
    Client & operator=(const Client & other);

    void                        readRequest();
    void                        generateResponse();
    void                        sendResponse();
    void                        closeConnection();

    states                      getState(void) const;
    int                         getFd() const;

 private:
    states                      _state;
    int                         _fd;
    std::vector<unsigned char>  _buf;
    std::string                 _response;
    std::string                 _request;

    Client(void);
    friend bool operator< (Client const& lhs, Client const& rhs) {
        return lhs.getFd() < rhs.getFd();
    }
};


#endif
