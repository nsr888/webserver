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
	return ft_strtrim(dst.substr(start + 1, dst.length()), " \t"); 
}

Location	Parser::getLocation(std::vector<std::string> temp, int i) {
	Location newLoc;
	newLoc._locationName = getArgument(temp[i], ft_strchr(temp[i], '/'));
	i++;
	while (temp[i].find("server:", 0, 7) == std::string::npos && temp[i].find("location:", 0, 9) == std::string::npos && i < (temp.size() - 1)) {
		if (temp[i].find("root:", 0, 5) != std::string::npos) {
			newLoc._path = getArgument(temp[i], ft_strchr(temp[i], ':'));
		}
		else if (temp[i].find("index:", 0, 6) != std::string::npos) {
			newLoc._file = getArgument(temp[i], ft_strchr(temp[i], ':'));
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
			if (temp[i].find("DELETE", 0, 6) != std::string::npos) {
				newLoc._delete = 1;
			}
		}
		i++;
	}
	return (newLoc);
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
			new_config.setHost(getArgument(temp[i], ft_strchr(temp[i], ':')));
		}
		else if (temp[i].find("port:", 0, 5) != std::string::npos) {
			new_config.setPort(ft_atoi(getArgument(temp[i], ft_strchr(temp[i], ':')).c_str()));
		}
		else if (temp[i].find("server_name:", 0, 12) != std::string::npos) {
			new_config.setServerName(getArgument(temp[i], ft_strchr(temp[i], ':')));
		}
		else if (temp[i].find("MaxBodySize:", 0, 12) != std::string::npos) {
			new_config.setMaxBodySize(ft_atoi(getArgument(temp[i], ft_strchr(temp[i], ':')).c_str()));
		}
		else if (temp[i].find("location:", 0, 9) != std::string::npos) {
			new_config.pushNewLocation(getLocation(temp, i));
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
		// кидаем исключение об ошибке чтения, пока заглушка
		std::cout << "Reading error" << std::endl;
	}
	fd.close();
	while (_isOneMoreServer == 1) {
		temp_config.push_back(get_config(temp));
	}
	return (temp_config);
}