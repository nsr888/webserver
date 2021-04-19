#ifndef PARSER_HPP
# define PARSER_HPP
# include <string>
extern "C" {
    char    *ft_strchr(const char *s, int c);
    char    *ft_strrchr(const char *s, int c);
}

class Parser {
public:
	static void fileParser(std::string& file);
	static void configParser(std::string& config);
	static std::string getArgument(const std::string &dst, int start);
	static void	setRequestLimits(const std::string& body_size);
	static void	setAutoIndex(const std::string& autoindex);
	static void	setRoot(const std::string& root);
	static void	setIndex(const std::string& index);
	static void	setAllowMethods(const std::string& allow_methods);
	static void	setCgiPath(const std::string& path_and_file);
	static void parseLocationParam(std::vector<std::string> &config, size_t &i, VirtualServer &server);
	static void parseServerParam(std::vector<std::string> &config, size_t &index, VirtualServer &server);
};

#endif
