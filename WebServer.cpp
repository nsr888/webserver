#include "WebServer.hpp"

WebServer::WebServer(const WebServer & other) { *this = other; }

WebServer::WebServer(void) { }

WebServer::WebServer(const Setting & config)
    : _ls()
    , _config(config)
    , _clients()
    , _addr() {
        _clients.reserve(25);
    }

WebServer::~WebServer(void) { }

WebServer & WebServer::operator=(const WebServer & other) {
    this->_config = other._config;
    this->_ls = other._ls;
    this->_clients = other._clients;
    this->_addr = other._addr;
    return *this;
}

int WebServer::getLs() const { return this->_ls; }

Setting & WebServer::getConfig() { return this->_config; }

sockaddr_in WebServer::getAddr() const {
    return this->_addr;
}

std::vector<Client>& WebServer::getClients() {
    return this->_clients;
}

void WebServer::appendClient(Client client) {
    this->_clients.push_back(client);
}

void WebServer::initServer() {
    _ls = socket(AF_INET, SOCK_STREAM, 0);
    if (_ls < 0)
        throw std::runtime_error(std::string("socket: ") + strerror(errno));
    if (fcntl(_ls, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error(std::string("fcntl: ") + strerror(errno));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(_config.getPort());
    _addr.sin_addr.s_addr = INADDR_ANY;
    int yes = 1;
    if (setsockopt(_ls, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        throw std::runtime_error(std::string("setsockopt (SO_REUSEADDR) failed: ") + strerror(errno));
    if (setsockopt(_ls, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(int)) == -1)
        throw std::runtime_error(std::string("setsockopt(SO_REUSEPORT) failed: ") + strerror(errno));
    if (bind(_ls, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
        throw std::runtime_error(std::string("bind: ") + strerror(errno));
    if (listen(_ls, 150) < 0)
        throw std::runtime_error(std::string("listen: ") + strerror(errno));
    std::cout << "\033[0;32m" << "Webserver started " << "\033[0m";
    std::cout << "(port " << _config.getPort() << ", listen socket: " << _ls << ")" << std::endl;
}
