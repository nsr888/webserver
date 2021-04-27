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
# include "ProcessMethod.hpp"
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
    std::vector<char>                  	_buf;
    std::string                  		_body;
	size_t								_body_size;
	size_t								_header_size;
	bool								_error_flag;
	std::string							_real_path;

	void 								initCodeList();

public:
	Response();
	~Response();

	std::string 						getMessage(int code);
	t_response_start_line               getStartLine(void) const;
    std::vector<char>                   getBuf(void) const;
    std::vector<char> &                 getBuf(void);
    std::string                   		getBody(void) const;
    std::string &                 		getBody(void);
	std::string                   		getPath(void) const;
	std::string	&						getPath(void);
    std::map<std::string, std::string>  getHeader() const;
	int									getBodySize() const;
	int									getHeaderSize() const;

	void								setBody(const std::string &body);
	void								setBodySize(size_t len);
	void								setCode(int code);
	void								setErrorFlag(bool flag);
	void								setPath(std::string path);

	void								check_path(Request &request);
	void								check_error(const std::string &error_msg);
	void								check_syntax(Request &request);
	void								check_method(Request &request);
	void								check_authentication(Request &request);
	
	void								generateResponseMsg(Request &request);
	std::string							generateErrorMsg();
	void								addHeader(Request &request, std::string &headers);
	void								addBody(const std::string &error_msg);

	std::string							toString(int nbr);
	std::string 						get_time();
    void                                clear();	
};

#endif
