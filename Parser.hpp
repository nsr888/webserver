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
};

#endif
