#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include "Response.hpp"
# include "Request.hpp"

class ProcessMethod
{
private:
	Response		_response;
	Request			_request;
	std::string		_method;

public:
	ProcessMethod(Request &request, Response &respone, std::string method);
	~ProcessMethod();

	void			secretary_Request(); /* Функция секретарь для отправки в нужный метод */
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

};


#endif