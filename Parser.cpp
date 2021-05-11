#include "Parser.hpp"
#include "Setting.hpp"
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <fstream> 

Parser::Parser() {
	_isOneMoreServer = 1;
	_stringReaded = 0;
}

std::string Parser::getArgument(const std::string &dst, int start) { 
	return utils::ft_strtrim(dst.substr(start + 1, dst.length()), " \t"); 
}

std::string Parser::getArgumentCGI(const std::string &dst, int start) { 
	return utils::ft_strtrim(dst.substr(start, 3), "\""); 
}

Location	Parser::getLocation(std::vector<std::string> temp, int i) {
	Location newLoc;
	newLoc._autoindex = 0;
	newLoc._head = 0;
	newLoc._get = 0;
	newLoc._post = 0;
	newLoc._put = 0;
	newLoc._maxBodySizeMB = 0;
	newLoc._locationName = getArgument(temp[i], utils::ft_strchr(temp[i], ':'));
	i++;
	while (temp[i].find("server:", 0, 7) == std::string::npos && 
            temp[i].find("location:", 0, 9) == std::string::npos && 
            static_cast<size_t>(i) < (temp.size() - 1)) 
	{
		if (temp[i].find("root:", 0, 5) != std::string::npos) {
			newLoc._path = getArgument(temp[i], utils::ft_strchr(temp[i], ':'));
		}
		else if (temp[i].find("autoindex:", 0, 10) != std::string::npos) {
			if ((getArgument(temp[i], utils::ft_strchr(temp[i], ':')) == "on"))
				newLoc._autoindex = 1;
			}
		else if (temp[i].find("index:", 0, 6) != std::string::npos) {
			newLoc._file = getArgument(temp[i], utils::ft_strchr(temp[i], ':'));
		}
		else if (temp[i].find("error_page:", 0, 11) != std::string::npos) {
			newLoc._ErrorPage = getArgument(temp[i], utils::ft_strchr(temp[i], ':'));
		}
		else if (temp[i].find("MaxBodySize:", 0, 12) != std::string::npos) {
			newLoc._maxBodySizeMB = ft_atoi(getArgument(temp[i], utils::ft_strchr(temp[i], ':')).c_str());
		}
		else if (temp[i].find("allow_methods:", 0, 14) != std::string::npos) {
			if (temp[i].find("GET", 0, 3) != std::string::npos) {
				newLoc._get = 1;
			}
			if (temp[i].find("PUT", 0, 3) != std::string::npos) {
				newLoc._put = 1;
			}
			if (temp[i].find("POST", 0, 4) != std::string::npos) {
				newLoc._post = 1;
			}
			if (temp[i].find("HEAD", 0, 4) != std::string::npos) {
				newLoc._head = 1;
			}
		}
		i++;
	}
	return (newLoc);
}

CGI	Parser::getCGI(std::vector<std::string> temp, int i) {
	CGI new_cgi;
	new_cgi._fileType = getArgumentCGI(temp[i], utils::ft_strchr(temp[i], '\"'));
	new_cgi._path = getArgument(temp[i], utils::ft_strchr(temp[i], ':'));
	return (new_cgi);
}

Setting Parser::get_config(std::vector<std::string> temp) {
	Setting	new_config;
	size_t size = temp.size();
	size_t i = _stringReaded;
	while(temp[i].find("server:") == std::string::npos)
		i++;
	i++;
	while(temp[i].find("server:") == std::string::npos && i < (size -1)) {
		if(temp[i].find("host:", 0, 5) != std::string::npos) {
			new_config.setHost(getArgument(temp[i], utils::ft_strchr(temp[i], ':')));
		}
		else if (temp[i].find("port:", 0, 5) != std::string::npos) {
			new_config.setPort(ft_atoi(getArgument(temp[i], utils::ft_strchr(temp[i], ':')).c_str()));
		}
		else if (temp[i].find("server_name:", 0, 12) != std::string::npos) {
			new_config.setServerName(getArgument(temp[i], utils::ft_strchr(temp[i], ':')));
		}
		else if (temp[i].find("location:", 0, 9) != std::string::npos) {
			new_config.pushNewLocation(getLocation(temp, i));
		}
		else if (temp[i].find("cgi", 0, 3) != std::string::npos) {
			new_config.pushNewCGI(getCGI(temp, i));
		}
		i++;
	}
	if (temp[i].find("server:") == std::string::npos) {
		_isOneMoreServer = 0;
	}
	else {
		_isOneMoreServer = 1;
		_stringReaded = i;
	}
	return new_config;
	
}

void	Parser::showConfig(std::vector<Setting> config) {
	size_t r = 0;
    int i = 0;
    int k = 0;
    while (r < config.size()) {
        i = 0;
        std::cout << "Config #" << (r + 1) << std::endl << std::endl;
        std::cout << "Host: " << config[r].getHost() << std::endl;
        std::cout << "Port: " << config[r].getPort() << std::endl;
        std::cout << "Server name: " << config[r].getServerName() << std::endl << std::endl;

        k = 0;
        while(k < config[r].getCGISize()) {
            std::cout << "CGI #" << (k + 1) << std::endl;
            std::cout << "File type: " << config[r].getCGIType(k) << std::endl;
            std::cout << "Path: " << config[r].getCGIPath(k) << std::endl << std::endl;
            k++;
        }

        while (i < config[r].getLocationSize()) { 
            std::cout << "Location #" << (i + 1) << std::endl;
            std::cout << "Name: " << config[r].getLocationName(i) << std::endl;
            std::cout << "Path: " << config[r].getLocationPath(i) << std::endl;
            std::cout << "index: " << config[r].getLocationFile(i) << std::endl;
            std::cout << "Body Size: " << config[r].getLocationMaxBodySize(i) << std::endl;
            std::cout << "Autoindex: " << config[r].getLocationAutoindex(i) << std::endl;
            std::cout << "Allow methods:" << std::endl;
            std::cout << "Put - " << config[r].getLocationPut(i) << std::endl;
            std::cout << "Post - " << config[r].getLocationPost(i) << std::endl;
            std::cout << "Head - " << config[r].getLocationHead(i) << std::endl;
            std::cout << "Get - " << config[r].getLocationGet(i) << std::endl;
            std::cout << "Body Size: " << config[r].getLocationMaxBodySize(i) << std::endl << std::endl;;
            i++;
        }
        r++;
    }
}

std::vector<Setting> Parser::startParsing(const char *config_file) {
	std::vector<Setting> temp_config;
	std::fstream fd;
	std::vector<std::string> temp;
	std::string line;
	_isOneMoreServer = 1;
	_stringReaded = 0;
	fd.open(config_file, std::fstream::in);
	if (fd.is_open()) {
		while (getline(fd, line)) {
			temp.push_back(line);
		}
	}
	else {
		std::cout << "Reading error" << std::endl;
	}
	fd.close();
	while (_isOneMoreServer == 1)
		temp_config.push_back(get_config(temp));
	return (temp_config);
}

