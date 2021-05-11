#ifndef PARSER_HPP
# define PARSER_HPP
# include <string>
# include <vector>
# include "Setting.hpp"
# include "utils.hpp"

extern "C" {
    int 	ft_atoi(const char *str);
	char	*ft_strtrim(char *s1, char const *set);
	char	*ft_strdup(const char *s1);
	char	*ft_substr(char const *s, unsigned int start, size_t len);
	size_t	ft_strlen(const char *s);

}

class Parser {
private:
	int		_isOneMoreServer;
	size_t	_stringReaded;


public:
	
	Parser(void);

	std::vector<Setting>	startParsing(const char *config_file);
	Setting					get_config(std::vector<std::string> temp);
	CGI						getCGI(std::vector<std::string> temp, int i);
	static std::string		getArgument(const std::string &dst, int start);
	std::string 			getArgumentCGI(const std::string &dst, int start);
	Location				getLocation(std::vector<std::string> temp, int i);
	void					showConfig(std::vector<Setting> config);
	int						checkConfig(std::vector<Setting> config);
	
	~Parser() {};
};

#endif
