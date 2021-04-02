#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <sys/socket.h>
# include <vector>
# include <string>
# include <iostream>

class Client {
public:
    Client(int fd);
    ~Client(void);
    Client(const Client & other);
    Client & operator=(const Client & other);

    void            processRequest(const char * request);
    bool            WantToRead(void) const;
    bool            WantToWrite(void) const;
    int             getFd() const;
    void            sendResponse();

private:
    int                         _fd;
    bool                        _want_to_read;
    bool                        _want_to_write;
    std::vector<unsigned char>  _buf;
    std::string                 _response;

    Client(void);
    friend bool operator< (Client const& lhs, Client const& rhs) { 
        return lhs.getFd() < rhs.getFd();
    };
};


#endif
