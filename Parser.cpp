#include "Parser.hpp"
#include "Setting.hpp"

void Parser::fileParser(std::string& file) {

};

static std::string FileParser::getArgument(const std::string &dst, int start) { 
	return ft_strtrim(dst.substr(start + 1, dst.length()), " \t"); 
};

Setting Parser::configParser(std::string& config) {
	Setting serverSetting;
	size_t size = config.size();
	size_t index = 0;
	size_t size = config.size();
	parseServerParam(config, index, serverSetting);
	for (size_t i = index; i <= size; ++i) {
		if (config[i].find("location:") != std::string::npos && checkIndent(config[i], 1))
			parseLocationParam(config, i, serverSetting);
		else if (config[i].empty()) continue;
		else { throw std::string ("Error config file"); }
		index = i;
		if (i == size)
			return serverSetting;
		if (config[i].find("server:") != std::string::npos) break;
	return serverSetting;
	}
};

void FileParser::parseLocationParam(std::vector<std::string> &config, size_t &i, VirtualServer &server) {
	Location location;
	std::string path = getArgument(config[i], ft_strchr(config[i], ':'));
	++i;
	while (i < config.size() && config[i].find("location:") == std::string::npos && config[i].find("server:") == std::string::npos) {
		if ((config[i].find("root:", 0, 5)) != std::string::npos)
			location.setRoot(getArgument(config[i], ft_strchr(config[i], ':')));
		else if ((config[i].find("autoindex:", 0, 10)) != std::string::npos)
			location.setAutoIndex(getArgument(config[i], ft_strchr(config[i], ':')));
		else if ((config[i].find("index:", 0, 6)) != std::string::npos)
			location.setIndex(getArgument(config[i], ft_strchr(config[i], ':')));
		else if ((config[i].find("allow_methods:", 0, 14)) != std::string::npos)
			location.setAllowMethods(getArgument(config[i], ft_strchr(config[i], ':')));
		else if ((config[i].find("limits_client_body_size:", 0, 24)) !=  std::string::npos)
			location.setRequestLimits(getArgument(config[i], ft_strchr(config[i], ':')));
		else if ((config[i].find("cgi_path:", 0, 9)) != std::string::npos)
			location.setCgiPath(getArgument(config[i], ft_strchr(config[i], ':')));
		else if (config[i].empty()) { ++i; continue; }
		if (!checkIndent(config[i], 2)) { std::cerr << "Error parse config file" << std::endl; break; }
		++i;
	}
	server.getLocation()[path] = location;
	if (i == config.size())
		return;
	if (config[i].find("location:") != std::string::npos)
		--i;
}

void FileParser::parseServerParam(std::vector<std::string> &config, size_t &index, VirtualServer &server) {
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
		if (!checkIndent(config[index], 1)) { std::cerr << "Error parse config file" << std::endl; break; }
		index++;
	}
}

void FileParser::setRequestLimits(const std::string &body_size) {
	_request_limits = std::stoi(ft_strtrim(body_size, " \t"));
}

void FileParser::setAutoIndex(const std::string &autoindex) {
	if (autoindex.find("on", 0) != std::string::npos)
		_autoindex = true;
	if (autoindex.find("off", 0) != std::string::npos)
		_autoindex = false;
}

void FileParser::setRoot(const std::string &root) {
	_root = ft_strtrim(root, " \t");
	if (*_root.rbegin() != '/')
		_root.push_back('/');
}

void FileParser::setIndex(const std::string &index) {
	_index =  ft_strtrim(index, " \t");
}

void FileParser::setAllowMethods(const std::string &allow_methods) {
	_allow_methods.insert(_allow_methods.begin(),ft_strtrim(allow_methods, " \t"));
}

bool FileParser::checkAllowMethod(const char *method) {
	std::vector<std::string>::iterator begin = _allow_methods.begin();
	while (begin != _allow_methods.end()) { /
		if ((*begin).find(method) != std::string::npos)
			break;
		else if (begin == _allow_methods.end() - 1 && (*begin).find(method) == std::string::npos)
			return false;
		begin++;
	}
	return true;
}

void FileParser::setCgiPath(const std::string &path_and_file) {
	size_t pos;
	if ((pos = path_and_file.find(' ')) == std::string::npos)
		std::cerr << "Error cgi_path" << std::endl;
	_cgi.insert(std::make_pair(path_and_file.substr(0, pos), path_and_file.substr(pos + 1, path_and_file.size())));
}
