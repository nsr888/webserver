#ifndef PARSER_HPP
# define PARSER_HPP
# include <string>

class Parser {
public:
	static void fileParser(std::string& file);
	static void configParser(std::string& config);
	static std::string getArgument(const std::string &dst, int start);
};

#endif