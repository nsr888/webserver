#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# define HTTP (std::string)"HTTP/1.1"
# define SPACE " "
# define CRLF "\r\n"

# include <iostream>
# include <iterator>
# include <string>
# include <vector>
# include <map>
# include <fcntl.h>
# include <cstdlib>
# include <unistd.h>
# include "Request.hpp"
extern "C" {
    char *ft_itoa(int n);
}

typedef struct s_response_start_line{
    std::string     code;
    std::string     message;
    std::string     http_version;
} t_response_start_line;

class Response
{
private:
	std::map<int,std::string>			_code_list;
	int									_code;
	t_response_start_line				_start_line;
	std::map<std::string, std::string>  _header;
    std::string                   		_buf;
    std::string                  		_body;
	size_t								_body_size;
	size_t								_header_size;

	void 								initCodeList();

public:
	Response();
	~Response();

	std::string 						getMessage(int code);
	t_response_start_line               getStartLine(void) const;
    std::string                   		getBuf(void) const;
    std::string &                 		getBuf(void);
    std::string                   		getBody(void) const;
    std::string &                 		getBody(void);
    std::map<std::string, std::string>  getHeader() const;
	int									getBodySize() const;
	int									getHeaderSize() const;

	void								setHeader();
	void								setBody();
	void								setCode(int code);

	void								generateResponse1(Request &request, int error_flag, const std::string &path);
	std::string							generateErrorMsg(int error_flag, const std::string &path);
	void								check_error(const std::string &error_msg);
	std::string							toString(int nbr);
	void								addHeader(Request &request, std::string &headers);
	void								addBody(const std::string &error_msg);






	
};

#endif
