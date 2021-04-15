#include "Setting.hpp"

void Setting::setHost(std::string& host) {
	_host = host;
}

void Setting::setPort(int port) {
	_port = port;
}

void Setting::setServerName(std::string& server_name) {
	_server_name = server_name;
}

void Setting::setErrorPage(std::string& error_page) {
	_errorPage = error_page;
}

