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

<<<<<<< Updated upstream
void Setting::setErrorPage(std::string& error_page) {
	_errorPage = error_page;
}
=======

void Setting::testfillError(void) {
	_errorLocation.push_back("Users");
	_errorLocation.push_back("anasyrov");
	_errorLocation.push_back("Documents");
	_errorLocation.push_back("21");
	_errorLocation.push_back("webserver");
	_errorLocation.push_back("webserver");
	_errorLocation.push_back("files");
	_errorLocation.push_back("error_page.html");
}

void Setting::testfillIndex(void) {
	_indexLocation.push_back("Users");
	_indexLocation.push_back("anasyrov");
	_indexLocation.push_back("Documents");
	_indexLocation.push_back("21");
	_indexLocation.push_back("webserver");
	_indexLocation.push_back("webserver");
	_indexLocation.push_back("files");
	_indexLocation.push_back("index.html");
}

void Setting::testfillFavicon(void) {
	_faviconLocation.push_back("Users");
	_faviconLocation.push_back("anasyrov");
	_faviconLocation.push_back("Documents");
	_faviconLocation.push_back("21");
	_faviconLocation.push_back("webserver");
	_faviconLocation.push_back("webserver");
	_faviconLocation.push_back("files");
	_faviconLocation.push_back("favicon.ico");
}

void Setting::testfillRoot(void) {
	_rootLocation.push_back("Users");
	_rootLocation.push_back("anasyrov");
	_rootLocation.push_back("Documents");
	_rootLocation.push_back("21");
	_rootLocation.push_back("webserver");
	_rootLocation.push_back("webserver");
}
>>>>>>> Stashed changes
