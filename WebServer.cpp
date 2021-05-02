#include "WebServer.hpp"

const std::string yel("\033[0;33m");
const std::string red("\033[0;31m");
const std::string grn("\033[0;32m");
const std::string blu("\033[0;34m");
const std::string gra("\033[0;30m");
const std::string res("\033[0m");

WebServer::WebServer(const WebServer & other) { *this = other; }

WebServer::WebServer(void) { }

WebServer::WebServer(const Setting & config)
    : _ls()
    , _config(config)
    , _clients()
    , _addr() { }

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
    fcntl(_ls, F_SETFL, O_NONBLOCK);
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
    listen(_ls, 5);
    std::cout << grn << "Webserver started " << res << "(port "
        << _config.getPort() << ", listen socket: " << _ls << ")" << std::endl;
}
