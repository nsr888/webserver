#include "ProcessMethod.hpp"
#include "Parser.hpp"
#include "libft/libft.h"
#include "utils.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>

ProcessMethod::ProcessMethod()
{
}

ProcessMethod::~ProcessMethod()
{
}

void	ProcessMethod::secretary_Request(Request &request, Response &respone, Setting * config, const std::string &method)
{
	_response = &respone;
	_request = &request;
	_config = config;
	_method = method;
	_stat_num = stat(_response->getPath().c_str(), &_stat);

	int i = _response->getLocationRespond();

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
		else if (_config->getLocationHead(i))
			processHeadRequest(i);
		else
			_response->setCode(405);
	}
	if (method == "POST")
	{
		if (!_config->getLocationPost(i))
        {
            _response->setCode(405);
            return;
        }
        /* if (_request->getHeader()["Content-Length"] == "0" */
		    /* || !_config->getLocationPost(i)) */
        /* { */
        /*     _response->setCode(500); */
        /*     return; */
        /* } */
		if (i == -1)
        {
			_response->setCode(404);
            return;
        }
        size_t max_body_size = _config->getLocationMaxBodySize(i);
        if (max_body_size && _request->getBody().size() > max_body_size)
        {
            _response->setCode(413);
            return;
        }
        std::string exec_prog = getCGI();
        if (exec_prog != "")
        {
            _execCGI(exec_prog);
            return;
        }
        processGetRequest(i);
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
    {
        _response->setBody(generateAutoindex(_response->getPath()));
        _response->setContentType("html");
    }
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

std::string	ProcessMethod::getCGI()
{
    std::string exec_prog;
    for (int i = 0; i < _config->getCGISize(); ++i)
    {
        if (_config->getCGIType(i) == _response->getTargetFile().second)
           exec_prog = _config->getCGIPath(i); 
    }
    return exec_prog;
}

void	ProcessMethod::processPutRequest()
{
	int fd = 0;
	size_t check = 0;
	std::string request_body(_request->getBody().begin(),_request->getBody().end());
	
    if (_request->getHeader()["Content-Length"] == "0")
    {
		_response->setCode(405);
        return;
    }
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

void ProcessMethod::_execCGI(const std::string & exec_prog)
{
    std::vector<char*> envVector;
    /* envVector.push_back(const_cast<char*>("AUTH_TYPE=")); */
    std::string content_length = "CONTENT_LENGTH=" + _request->getHeader()["Content-Length"];
    envVector.push_back(const_cast<char*>(content_length.c_str()));
    envVector.push_back(const_cast<char*>("CONTENT_TYPE=application/x-www-form-urlencoded"));
    envVector.push_back(const_cast<char*>("GATEWAY_INTERFACE=CGI/1.1"));
    /* std::string path_info = "PATH_INFO=" + _response->getPath(); */
    std::string path_info = "PATH_INFO=" + _request->getStartLine().request_target;
    envVector.push_back(const_cast<char*>(path_info.c_str()));
    std::string path_translated = "PATH_TRANSLATED=" + _response->getPath();
    envVector.push_back(const_cast<char*>(path_translated.c_str()));
    /* envVector.push_back(const_cast<char*>("QUERY_STRING=")); */
    std::string remote_addr = "REMOTE_ADDR=" + std::string(_config->getHost());
    envVector.push_back(const_cast<char*>(remote_addr.c_str()));
    /* envVector.push_back(const_cast<char*>("REMOTE_IDENT=")); */
    /* envVector.push_back(const_cast<char*>("REMOTE_USER=")); */
    std::string request_method = "REQUEST_METHOD=" + _request->getStartLine().method;
    envVector.push_back(const_cast<char*>(request_method.c_str()));
    std::string request_uri = "REQUEST_URI=" + _request->getStartLine().request_target;
    envVector.push_back(const_cast<char*>(request_uri.c_str()));
    std::string script_name = "SCRIPT_NAME=" + _response->getTargetFile().first;
    envVector.push_back(const_cast<char*>(script_name.c_str()));
    envVector.push_back(const_cast<char*>("REDIRECT_STATUS=200"));
    std::string server_name = "SERVER_NAME=" + _config->getServerName();
    envVector.push_back(const_cast<char*>(server_name.c_str()));
    char *port = ft_itoa(_config->getPort());
    std::string server_port = "SERVER_PORT=" + std::string(port);
    free(port);
    envVector.push_back(const_cast<char*>(server_port.c_str()));
    envVector.push_back(const_cast<char*>("SERVER_PROTOCOL=HTTP/1.1"));
    envVector.push_back(const_cast<char*>("SERVER_SOFTWARE=1.0"));
    /* std::string script_filename = "SCRIPT_FILENAME=" + _response->getPath(); */
    /* envVector.push_back(const_cast<char*>(script_filename.c_str())); */

    std::map<std::string, std::string> headers = _request->getHeader();
    std::map<std::string, std::string>::iterator it = headers.begin();
    std::string x_header_upper;
    for(;it != headers.end(); ++it)
    {
        if ((it->first)[0] == 'X')
        {
            std::locale loc;
            std::string x_header = "HTTP_" + it->first + "=" + it->second;
            std::replace(x_header.begin(), x_header.end(), '-', '_');
            for (size_t i = 0; i < x_header.length(); ++i)
                x_header_upper += ft_toupper(x_header[i]);
            envVector.push_back(const_cast<char*>(x_header_upper.c_str()));
        }
    }
    envVector.push_back(0);
    char **env = envVector.data();

	if (_config->getDebugLevel() > 1)
    {
        std::cout << utils::GRA << "------" << " CGI envirements start " << "------" << utils::RES << std::endl;
        for (size_t i = 0; envVector[i] != 0; ++i)
            std::cout << "  " << envVector[i] << std::endl;
        std::cout << utils::GRA << "------" << " CGI envirements end " << "------" << utils::RES << std::endl;
    }

    char request_body[] = "./tmp/request_body.tmp";
    std::vector<char> body = _request->getBody();
    utils::log(*_config, __FILE__, "body_size: ", body.size());
    utils::write_file_raw(request_body, body);

    /* https://stackoverflow.com/a/39395978 */
    int child_to_parent[2];
    /* int parent_to_child[2]; */
    if (pipe(child_to_parent) == -1)
        utils::e_throw("_cgi pipe", __FILE__, __LINE__);
    /* if (fcntl(parent_to_child[1], F_SETFL, O_NONBLOCK) < 0) */
    /*     throw std::runtime_error(std::string("execCGI fcntl: ") +
     * strerror(errno)); */
    /* if (fcntl(child_to_parent[0], F_SETFL, O_NONBLOCK) < 0) */
    /*     throw std::runtime_error(std::string("execCGI fcntl: ") +
     * strerror(errno)); */

    pid_t pid = fork();

    if (pid == 0)
    {
        /* In child process */
        /* Output setup */
        dup2(child_to_parent[1], STDOUT_FILENO);
        close(child_to_parent[0]);

        /* Input setup */
        /* dup2(parent_to_child[0], STDIN_FILENO); */
        /* close(parent_to_child[1]); */

        /* char *arg[] = { const_cast<char*>(exec_prog.c_str()), 0 }; */

        /* EXECVE */

        int fd2 = open(request_body, O_RDONLY);
        if (fd2 < 0)
            utils::e_throw("_cgi open fd2", __FILE__, __LINE__);
        if (dup2(fd2, STDIN_FILENO) < 0)
            utils::e_throw("_cgi dup2 fd2", __FILE__, __LINE__);
        close(fd2);

        /* std::cerr << "execve" << std::endl; */
        char *arg[] = {const_cast<char *>(_response->getPath().c_str()), 0};
        int execve_ret = execve(exec_prog.c_str(), &arg[0], &env[0]);
        /* close(parent_to_child[0]); */
        if (execve_ret < 0)
        {
            std::cerr << "EXIT_FAILURE" << std::endl;
            exit(EXIT_FAILURE);
        }
        else
            exit(EXIT_SUCCESS);
    } else {
        /* In parent process */
        close(child_to_parent[1]);
        /* close(parent_to_child[0]); */

        /* READ START */
        utils::log(*_config, __FILE__, "read to parent start");
        char                line[10000];
        int                 m;
        std::vector<char>   buf;
        while (1)
        {
            m = read(child_to_parent[0], line, 10000);
            if (m == -1)
                continue;
            else if (m == 0)
                break;
            buf.insert(buf.end(), line, line + m);
        }

        /* std::vector<char>::iterator it; */
        /* it = find (buf.begin(), buf.end(), 4); */
        /* if (it != buf.end()) */
        /* { */
        /*     std::cout << "Char 0x04 found in myvector \n"; */
        /*     buf.erase(it, buf.end()); */
        /* } */

        utils::log(*_config, __FILE__, "read buf size: ", buf.size());
        /* std::cout << std::string(buf.begin(), buf.end()) << std::endl; */
        /* READ END */

        /* WRITE CGI RESPONSE FOR 100000 */
        /* if (_request->getHeaderField("Content-Length") == "100000") */
        /* { */
        /*     char cgi_response[] = "./tmp/cgi_response.XXXXXX"; */
        /*     mkstemp(cgi_response); */
        /*     utils::write_file_raw(cgi_response, buf); */
        /* } */

        while (waitpid(-1, 0, 0) != -1)
            ;
        /* int status; */
        /* if (waitpid(pid, &status, 0) < 0) */
        /*     throw std::runtime_error(std::string("execCGI waitpid") + strerror(errno)); */
        if (Request::bufContains(&buf, "\r\n\r\n"))
        {
            std::vector<char> headers_buf_from_cgi;
            headers_buf_from_cgi = Request::deleteHeaderInBuf(&buf);
            std::map<std::string, std::string> header_map;
            header_map = utils::parseBufToHeaderMap(_response->getHeader(),
                    headers_buf_from_cgi);
            _response->setHeader(header_map);
        }
        /* std::cout << "buf.size(): " << buf.size() << std::endl; */
        _response->setBody(std::string(buf.begin(), buf.begin() + buf.size()));
        _response->setCode(200);
        /* if (_request->getHeaderField("Content-Length") != "100000") */
        unlink(request_body);
        /* Clear memory used by _buf, using clear() method insufficient */
        /* https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Clear-and-minimize */
        std::vector<char>().swap(buf);
    }
}
