#ifndef PROCESSMETHOD_HPP
# define PROCESSMETHOD_HPP

# include <iostream>
# include "Request.hpp"
# include "Response.hpp"
# include "Setting.hpp"
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <dirent.h>

class Response;

class ProcessMethod
{
private:
	Response*		_response;
	Request*		_request;
	Setting*		_config;
	std::string		_method;
	struct stat		_stat;
	int				_stat_num;

public:
	ProcessMethod();
	~ProcessMethod();

	void			secretary_Request(Request &request, Response &respone, Setting &config, const std::string &method);
	void			generateBody();
	
	void			processGetRequest();
	void			processHeadRequest();
	void			processPostRequest();
	void			processPutRequest();
	void			processDeleteRequest();
	void			processOptionsRequest();
	void			processTraceRequest();

	void			isLocationHasIndex();
	void			isListingAllowed();
	void			getFile();
	void			generateListing();
	void			generateCGI();
	std::string		readPath(std::string path);
	std::string		generateAutoindex(std::string path);
	int				numberInLocation();

};

#endif
