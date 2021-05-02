#include "Setting.hpp"

Setting::Setting(void) { }
Setting::~Setting(void) { }

Setting::Setting(const Setting & other) { *this = other; }

Setting & Setting::operator=(const Setting & other) {
    _server_name = other._server_name;
    _host = other._host;
    _port = other._port;
    _rootLocation = other._rootLocation;
    _errorLocation = other._errorLocation;
    _indexLocation = other._indexLocation;
    _faviconLocation = other._faviconLocation;
    _maxBodySizeMB = other._maxBodySizeMB;
    _location = other._location;
    return *this;
}

void Setting::setHost(std::string host) {
	_host = host;
}

void Setting::setPort(int port) {
	_port = port;
}

void Setting::setServerName(std::string server_name) {
	_server_name = server_name;
}

void Setting::setMaxBodySize(int BodySize) {
	_maxBodySizeMB = BodySize;
}

void Setting::pushNewLocation(Location temp) {
	_location.push_back(temp);
}
