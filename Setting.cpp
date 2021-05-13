#include "Setting.hpp"

Setting::Setting(void) { }

Setting::~Setting(void) { }

Setting::Setting(const Setting & other) { *this = other; }

Setting & Setting::operator=(const Setting & other) {
    _server_name = other._server_name;
    _host = other._host;
    _port = other._port;
    _location = other._location;
    _CGI = other._CGI;
    _debug = other._debug;
    return *this;
}

void    Setting::setHost(std::string host) {
	_host = host;
}

void    Setting::setPort(int port) {
	_port = port;
}

void    Setting::setServerName(std::string server_name) {
	_server_name = server_name;
}

void    Setting::pushNewLocation(Location temp) {
	_location.push_back(temp);
}

void    Setting::pushNewCGI(CGI temp) {
	_CGI.push_back(temp);
}

void    Setting::setDebugLevel(int debug) {
    _debug = debug;
}
