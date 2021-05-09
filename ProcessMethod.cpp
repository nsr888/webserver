#include "ProcessMethod.hpp"

ProcessMethod::ProcessMethod()
{
}

ProcessMethod::~ProcessMethod()
{
}

void	ProcessMethod::secretary_Request(Request &request, Response &respone, Setting &config, const std::string &method)
{
	_response = &respone;
	_request = &request;
	_config = &config;
	_method = method;
	_stat_num = stat(_response->getPath().c_str(), &_stat);

	int i = numberInLocation();

	if (method == "GET")
	{
		if (i == -1)
			processGetRequest(-1);
		else if (_config->getLocationGet(i))
			processGetRequest(i);
		else
			_response->setCode(405);
	}
	if (method == "HEAD")
	{
		if (i == -1)
			processHeadRequest(-1);
		else if (_config->getLocationGet(i))
			processHeadRequest(i);
		else
			_response->setCode(405);
	}
	if (method == "POST")
	{
		if (i == -1)
			processPutRequest();
		else if (_config->getLocationPost(i))
			processPutRequest();
		else
			_response->setCode(405);
	}
	if (method == "PUT")
	{
		if (i == -1)
			processPutRequest();
		else if (_config->getLocationPut(i))
			processPutRequest();
		else
			_response->setCode(405);
	}
	if (method == "HEAD")
	{
		if (i == -1)
			processHeadRequest();
		else if (_config->getLocationHead(i))
			processHeadRequest();
		else
			_response->setCode(405);
	}
}

void	ProcessMethod::processGetRequest(int i)
{
	bool autoindex = true;

	if (i != -1 && _config->getLocationAutoindex(i) == 0)
		autoindex = false;
	_response->setCode(200);
	if (S_ISLNK(_stat.st_mode) || S_ISREG(_stat.st_mode))
		_response->setBody(readPath(_response->getPath()));
	else if (S_ISDIR(_stat.st_mode) && autoindex == true)
		_response->setBody(generateAutoindex(_response->getPath()));
	else
		_response->setCode(404);
}

void	ProcessMethod::processHeadRequest(int i)
{
	bool autoindex = true;

	if (i != -1 && _config->getLocationAutoindex(i) == 0)
		autoindex = false;
	_response->setCode(200);
	if (S_ISDIR(_stat.st_mode) && autoindex == false)
		_response->setCode(404);
}

void	ProcessMethod::processPostRequest()
{
	
}

void	ProcessMethod::processPutRequest()
{
	int fd = 0;
	size_t check = 0;
	std::string request_body(_request->getBody().begin(),_request->getBody().end());
	
	if (S_ISDIR(_stat.st_mode))
		_response->setCode(404);
	if ((fd = open(_response->getPath().c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0)
		_response->setCode(500);
	else
	{
		check = write(fd, request_body.c_str(), request_body.length());
		if (check != request_body.length()) {
			_response->setCode(200);
			std::cout << "Write error" << std::endl;
			close(fd);
		}
		else if (_stat_num != -1)
			_response->setCode(200);
		else
			_response->setCode(201);
		close(fd);
	}
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
	struct dirent*	entry = 0;

	if (directory)
	{
		autoindex.append("<html><head><title>Index of </title></head><body><h1>Index of </h1><br><hr><a href=\"../\">../</a><br>");
		while ((entry = readdir(directory)) != 0) 
			autoindex.append("<a href=\"" + std::string(entry->d_name) + "\">" + std::string(entry->d_name) + "</a><br>");
		autoindex.append("</body></html>");
	}
	return (autoindex);
}

int	ProcessMethod::numberInLocation()
{
	size_t 		len = _config->getLocationSize();
	std::string path = _response->getPath();

	for (size_t i = 0; i < len; i++)
	{
		if (_config->getLocationPath(i) == path)
			return (i);
	}
	return (-1);
}
