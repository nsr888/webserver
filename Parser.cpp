#include "Parser.hpp"
#include "Setting.hpp"

void Parser::fileParser(std::string& file) {

};

static std::string FileParser::getArgument(const std::string &dst, int start) { 
	return ft_strtrim(dst.substr(start + 1, dst.length()), " \t"); 
};

void Parser::configParser(std::string& config) {
	Setting serverSetting;
	size_t size = config.size();
	size_t index = 0;
	while (config[index].find("location:") == std::string::npos && config[index].find("server:") == std::string::npos) {
		if (config[index].find("host:", 0, 5) != std::string::npos)
			server.setHost(getArgument(config[index], ft_strchr(config[index], ':')));
		else if (config[index].find("port:", 0, 5) != std::string::npos)
			server.setPort(getArgument(config[index], ft_strchr(config[index], ':')));
		else if (config[index].find("server_name:", 0, 12) != std::string::npos)
			server.setServerName(getArgument(config[index], ft_strchr(config[index], ':')));
		else if (config[index].find("error_page:", 0, 11) != std::string::npos)
			server.setErrorPage(getArgument(config[index], ft_strchr(config[index], ':')));
		else if (config[index].empty()) { ++index; continue; }
		index++;
};
