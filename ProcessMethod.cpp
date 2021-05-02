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

	_stat_num = stat(_response->getPath().c_str(), &_stat);

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
	_response->setCode(200);
	if (S_ISLNK(_stat.st_mode) || S_ISREG(_stat.st_mode))
		_response->setBody(readPath(_response->getPath()));
	else if (S_ISDIR(_stat.st_mode) /* && autoindex включен */)
		_response->setBody(generateAutoindex(_response->getPath()));
	else
		_response->setCode(404);
}

void	ProcessMethod::processHeadRequest()
{
	
	
}

void	ProcessMethod::processPostRequest()
{
	
}

void	ProcessMethod::processPutRequest()
{
	int fd = 0;
	std::string request_body(_request->getBody().begin(),_request->getBody().end());
	
	if (S_ISDIR(_stat.st_mode))
		_response->setCode(404);
	if ((fd = open(_response->getPath().c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0)
		_response->setCode(500);
	else
	{
		write(fd, request_body.c_str(), request_body.length());
		if (_stat_num != -1)
			_response->setCode(200);
		else
			_response->setCode(201);
		close(fd);
	}
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

std::string ProcessMethod::generateAutoindex(std::string path) 
{
	std::string 	autoindex;
	DIR*			directory = opendir(path.c_str());
	struct dirent*	entry = nullptr;

	if (directory)
	{
		autoindex.append("<html><head><title>Index of </title></head><body><h1>Index of </h1><br><hr><a href=\"../\">../</a><br>");
		while ((entry = readdir(directory)) != nullptr) 
			autoindex.append("<a href=\"" + std::string(entry->d_name) + "\">" + std::string(entry->d_name) + "</a><br>");
		autoindex.append("</body></html>");
	}
	return (autoindex);
}
