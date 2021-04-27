#ifndef PROCESSMETHOD_HPP
# define PROCESSMETHOD_HPP

# include <iostream>
# include "Request.hpp"
# include "Response.hpp"

class Response;

class ProcessMethod
{
private:
	Response*		_response;
	Request*		_request;
	std::string		_method;

public:
	ProcessMethod();
	~ProcessMethod();

	void			secretary_Request(Request &request, Response &respone, const std::string &method); /* Функция секретарь для отправки в нужный метод */
	void			generateBody(); /* функция должна читать по пути и в итоге записать это в Response::setBody(std::string) и Response::setBodySize(int) */
	
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

};

#endif
