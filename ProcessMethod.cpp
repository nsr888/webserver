#include "ProcessMethod.hpp"

ProcessMethod::ProcessMethod()
{
}

ProcessMethod::~ProcessMethod()
{
}

void	ProcessMethod::secretary_Request(Request &request, Response &respone, const std::string &method)
{
	_response = &respone;
	_request = &request;
	_method = method;

	if (method == "GET")
		processGetRequest();
	if (method == "HEAD")
		processHeadRequest();
	if (method == "POST")
		processPostRequest();
	if (method == "PUT")
		processPutRequest();
	if (method == "DELETE")
		processDeleteRequest();
	if (method == "OPTIONS")
		processOptionsRequest();
	if (method == "TRACE")
		processTraceRequest();
}

void	ProcessMethod::processGetRequest()
{
	std::string path = readPath(_response->getPath());

	_response->setCode(200);
	_response->setBody(path);
	_response->setBodySize(path.length());
}

void	ProcessMethod::processHeadRequest()
{
	
	
}

void	ProcessMethod::processPostRequest()
{
	
}

void	ProcessMethod::processPutRequest()
{
	
}

void	ProcessMethod::processDeleteRequest()
{
	
}

void	ProcessMethod::processOptionsRequest()
{
	
}

void	ProcessMethod::processTraceRequest()
{
	
}

std::string ProcessMethod::readPath(std::string path)
{
	int 		fd = open(path.c_str(), O_RDONLY);;
	char		buf[100];
	std::string body;

	if (!fd)
		std::cerr << "File doesn`t open" << std::endl;
	bzero(buf, 100);
	int pos = 0;
	while ((pos = read(fd, &buf, 100)) > 0)
		body.append(buf, pos);
	close(fd);
	return (body);
}
