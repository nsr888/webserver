#include "Setting.hpp"

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
