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
# include <sys/time.h>
# include <ctime>
# include "utils.hpp"
# include "Request.hpp"
# include "ProcessMethod.hpp"
# include "Parser.hpp"
# include <fstream>  
# include <dirent.h>

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
	std::string							_real_path;
    Setting*                            _config;
	int									_locationRespond;
	std::pair<std::string, std::string>	_target_file;

	void 								initCodeList();

public:
	Response();
	explicit Response(Setting * config);
	~Response();
    Response(const Response & other);
    Response & operator=(const Response & other);

	std::string 						getMessage(int code);
	t_response_start_line               getStartLine(void) const;
    std::vector<char>                   getBuf(void) const;
    std::vector<char> &                 getBuf(void);
    std::string                   		getBody(void) const;
	std::string                   		getPath(void) const;
	std::string	&						getPath(void);
    std::map<std::string, std::string>  getHeader() const;
	int									getBodySize() const;
	int									getHeaderSize() const;
	std::pair<std::string, std::string> getTargetFile() const;
	int									getLocationRespond();


	void								setBody(const std::string &body);
	void								setBodySize(size_t len);
	void								setCode(int code);
	void 								setContentType(std::string type);
	void								setTargetFile();
	void								setPath(std::string path);

	void								check_path(Request &request);
	void								check_error(const std::string &error_msg, Request &request);
	void								check_syntax(Request &request);
    void		                        check_auth(Request &request);
	void								check_method(Request &request);
	void								check_accept(Request &request);
	
	void								generateResponseMsg(Request &request);
	std::string							generateErrorMsg();
	void								addHeader(Request &request, std::string &headers);
	void								addBody(const std::string &error_msg);

	std::string							toString(int nbr);
	std::string 						get_time();
    void                                clear();	
    void                                setHeader(std::map<std::string, std::string> & map);
    void                                setBody(std::string & body);
	std::vector<std::string>			slashSplit(std::string forsplit);
	std::string							pathCompare(std::vector<std::string> requesty,std::vector<std::string> locationy);
	void								set_Allow_to_Header();
	void 								rootPath(void);
	std::string 						httpPath(Request &request);
	void 								checkExist(void);
	void								addLog(void);

};

#endif
