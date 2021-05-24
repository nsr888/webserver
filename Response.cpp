#include "Response.hpp"
#include "utils.hpp"

Response::Response()
    : _code_list()
    , _code(0)
    , _start_line()
    , _header()
    , _buf()
    , _body()
    , _body_size(0)
    , _header_size(0)
    , _real_path()
    , _config()
{ }

Response::Response(Setting * config)
    : _code_list()
    , _code(0)
    , _start_line()
    , _header()
    , _buf()
    , _body()
    , _body_size(0)
    , _header_size(0)
    , _real_path()
    , _config(config)
{
	if (this->_code_list.empty())
		initCodeList();
}

Response::~Response() { }

Response::Response(const Response & other) { *this = other; }

Response & Response::operator=(const Response & other) {
    _code_list = other._code_list;
    _code = other._code;
    _start_line = other._start_line;
    _header = other._header;
    _buf = other._buf;
    _body = other._body;
    _body_size = other._body_size;
    _header_size = other._header_size;
    _real_path = other._real_path;
    _config = other._config;
    return *this;
}

void Response::initCodeList()
{
    this->_code_list[0] = "CODE NOT SETTED IN RESPONSE.CPP !";
    this->_code_list[100] = "Continue";
    this->_code_list[101] = "Switching Protocols";
    this->_code_list[200] = "OK";
    this->_code_list[201] = "Created";
    this->_code_list[202] = "Accepted";
    this->_code_list[203] = "Non-Authoritative Information";
    this->_code_list[204] = "No Content";
    this->_code_list[205] = "Reset Content";
    this->_code_list[206] = "Partial Content";
    this->_code_list[300] = "Multiple Choices";
    this->_code_list[301] = "Moved Permanently";
    this->_code_list[302] = "Found";
    this->_code_list[303] = "See Other";
    this->_code_list[304] = "Not Modified";
    this->_code_list[305] = "Use Proxy";
    this->_code_list[307] = "Temporary Redirect";
    this->_code_list[400] = "Bad Request";
    this->_code_list[401] = "Unauthorized";
    this->_code_list[402] = "Payment Required";
    this->_code_list[403] = "Forbidden";
    this->_code_list[404] = "Not Found";
    this->_code_list[405] = "Method Not Allowed";
    this->_code_list[406] = "Not Acceptable";
    this->_code_list[407] = "Proxy Authentication Required";
    this->_code_list[408] = "Request Timeout";
    this->_code_list[409] = "Conflict";
    this->_code_list[410] = "Gone";
    this->_code_list[411] = "Length Required";
    this->_code_list[412] = "Precondition Failed";
    this->_code_list[413] = "Payload Too Large";
    this->_code_list[414] = "URI Too Long";
    this->_code_list[415] = "Unsupported Media Type";
    this->_code_list[416] = "Range Not Satisfiable";
    this->_code_list[417] = "Expectation Failed";
    this->_code_list[426] = "Upgrade Required";
    this->_code_list[500] = "Internal Server Error";
    this->_code_list[501] = "Not Implemented";
    this->_code_list[502] = "Bad Gateway";
    this->_code_list[503] = "Service Unavailable";
    this->_code_list[504] = "Gateway Timeout";
    this->_code_list[505] = "HTTP Version Not Supported";
}

std::string Response::getMessage(int code)
{
	return (_code_list.at(code));
}

t_response_start_line Response::getStartLine(void) const
{
	return _start_line;
}

std::vector<char> & Response::getBuf(void) 
{
	return _buf;
}

std::vector<char> Response::getBuf(void) const 
{
	return _buf;
}

std::string & Response::getPath(void) 
{
	return _real_path;
}

std::string Response::getPath(void) const 
{
	return _real_path;
}

void Response::setBody(std::string & body) 
{
	_body = body;
}

std::string Response::getBody(void) const 
{
	return _body;
}

std::map<std::string, std::string> Response::getHeader() const 
{
    return _header;
}

void Response::setHeader(std::map<std::string, std::string> & map)
{
    _header = map;
}

int	Response::getBodySize() const
{
	return _body_size;
}

int	Response::getHeaderSize() const
{
	return _header_size;
}

std::string	Response::toString(int nbr)
{
	char *ch_code = ft_itoa(nbr);
    if (!ch_code) {
        throw std::runtime_error(std::string("ft_itoa: ") + strerror(errno));
	}
	std::string status_code(ch_code);
	delete ch_code;

	return (status_code);
}

void		Response::generateResponseMsg(Request &request)
{
    std::string error_msg;
    std::string headers;

    utils::log(*_config, __FILE__, "generateResponseMsg");
    check_syntax(request);
    if (_code == 0) 
    {
         check_path(request);
         setTargetFile();
    }
    if (_code == 0) 
         check_auth(request);
	if (_code == 0)
		check_accept(request);
    if (_code == 0)
        check_method(request);
    if (_code >= 400)
    {
        error_msg = generateErrorMsg();
        check_error(error_msg, request);
    }

	//_body = "Hello world!"; /* Пока не понимаю из чего формируется боди, видимо нужна отдельная функция */
	//_body_size = _body.length(); /* Размер боди должен считаться когда формируется боди */

	addHeader(request, headers);
	headers.append(CRLF);

	std::string::iterator beg = headers.begin();
	std::string::iterator end = headers.end();

	while (beg != end)
	{
		_buf.push_back(*beg);
		++beg;
	}
    if (request.getStartLine().method != "HEAD")
        addBody(error_msg);
    if (_config->getDebugLevel() == -1)
        utils::log(*_config, "HTTP HEADER", _start_line.http_version 
                + " " + _start_line.code + " " + _start_line.message );
}

std::string	Response::generateErrorMsg()
{
    utils::log(*_config, __FILE__, "generateErrorMsg");
    std::string error;
    std::string	error_path = "./files/error.html";

	if (_code == 404)
	{
		int fd = open(error_path.c_str(), O_RDONLY);
		char buf[100];
		bzero(buf, 100);
		int pos = 0;
		while ((pos = read(fd, &buf, 100)) > 0)
			error.append(buf, pos);
		close(fd);
	}
	else
	{
		error.append(
				"<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" "
				"content=\"width=device-width, initial-scale=1.0\"><meta http-equiv=\"X-UA-Compatible\" "
				"content=\"ie=edge\"><title>" + toString(_code) + " " +
				getMessage(_code) + "</title><style>h1, "
												 "p {text-align: center;}</style></head><body><h1>" +
				toString(_code) + " " + getMessage(_code) +
				"</h1><hr><p>" + _config->getServerName() + "</p></body></html>");
	}
	return (error);
}

std::string	Response::pathCompare(std::vector<std::string> requesty,std::vector<std::string> locationy) {
	int i = 0;
	int match = 0;
	int loc = 0;
	while (i < static_cast<int>(locationy.size())) {
		if (requesty[0] == utils::ft_strtrim(_config->getLocationName(i), "/")) {
			match = 1;
			loc = 1;
			_locationRespond = i;
			break;
		}
		i++;
	}
	if (match == 0) {
		i = 0;
		while (i < static_cast<int>(locationy.size())) {
			if (requesty[0] == locationy[i]) {
				match = 1;
				break;
			}
		i++;
		}
	}
	if (match == 1 && loc == 0) {
		std::string ret;
		int r = 0;
		while (i >= r) {
			ret = ret + "/" + locationy[r];
			r++;
		}
		r = 1;
		while (r < static_cast<int>(requesty.size())) {
			ret = ret + "/" + requesty[r];
			r++;
		}
		return (ret);
	}
	else if (match == 1 && loc == 1) {
		std::string ret;
		int r = 1;
		ret = _config->getLocationPath(i);
		while (r < static_cast<int>(requesty.size())) {
			ret = ret + "/" + requesty[r];
			r++;
		}
		if (!_config->getLocationFile(i).empty() && requesty.size() == 1) {
			ret = ret + "/" + _config->getLocationFile(i);
		}
		return (ret);
	}
	else {
		return ("");
	}
}

std::vector<std::string>	Response::slashSplit(std::string forsplit) {
	std::vector<std::string> temp;
	forsplit = utils::ft_strtrim(forsplit, "/");
	if (forsplit.find("/") == std::string::npos) {
		temp.push_back(forsplit);
		return (temp);
	}
	while(forsplit.find("/") != std::string::npos) {
		temp.push_back(forsplit.substr(0,forsplit.find("/")));
		forsplit = forsplit.substr(forsplit.find("/"), (forsplit.size() - forsplit.find("/")));
		forsplit = utils::ft_strtrim(forsplit, "/");
	}
	temp.push_back(forsplit);
	return (temp);
}

std::vector<std::string> str_split(std::string str, std::string token)
{
    std::vector<std::string>result;
    while (str.size())
    {
        size_t index = str.find(token);
        if (index!=std::string::npos)
        {
            result.push_back(str.substr(0,index));
            str = str.substr(index + token.size());
            if(str.size() == 0) result.push_back(str);
        }
        else
        {
            result.push_back(str);
            str = "";
        }
    }
    return result;
}


void 	Response::rootPath(void) {
	int i = 0;
	while (i < _config->getLocationSize()) {
		if (_config->getLocationName(i) == "/") {
			_locationRespond = i;
			setPath(_config->getLocationPath(i));
			break;
		}
		i++;
	}
}

void 	Response::httpPath(Request &request) {
	t_start_line temp = request.getStartLine();
	temp.request_target = utils::ft_strtrim(temp.request_target, "http://");
	temp.request_target = utils::ft_strtrim(temp.request_target, _config->getServerName());
	temp.request_target = utils::ft_strtrim(temp.request_target, ":");
	temp.request_target = utils::ft_strtrim(temp.request_target, toString(_config->getPort()));
}

void 	Response::checkExist() {
	std::string temp = getPath();
	if (temp.find(".", 0, 1)) {
		const char *path = getPath().c_str();
		std::ifstream ifs;
		ifs.open (path, std::ifstream::in);
		if(!ifs) {
			setCode(404);
		}
		ifs.close();
	}
	else {
		const char *path = getPath().c_str();
		DIR* dir = opendir(path);
		if(!dir) {
			setCode(404);
		}
		else {
			closedir(dir);
		}
	}
}

void	Response::addLog() {
	if (_config->getDebugLevel() > 1) {
        utils::log(*_config, __FILE__, "setPath is " + getPath());
		if (_config->getDebugLevel() > 2) {
            utils::log(*_config, __FILE__, "Location in config for response is ", _locationRespond);
		}
	}
}

void	Response::check_path(Request &request)
{
    utils::log(*_config, __FILE__, "check_path");
	t_start_line temp = request.getStartLine();
	std::string tempo = utils::ft_strtrim(temp.request_target, "/");
	_locationRespond = -1;
	int i = 0;
	if (temp.request_target.find("http://", 0, 7) != std::string::npos) {
		httpPath(request);
	}
	if (temp.request_target == "/") {
		rootPath();
		setPath(_config->getLocationPath(i) + "/" + _config->getLocationFile(i));
		addLog();
		return;
	}
	while (i < _config->getLocationSize()) {
		if (_config->getLocationName(i) == temp.request_target) {
			setPath(_config->getLocationPath(i));
			_locationRespond = i;
			if (!_config->getLocationFile(i).empty()) {
				setPath(_config->getLocationPath(i) + "/" + _config->getLocationFile(i));
			}
		}
		i++;
	}
	if (temp.request_target.find("/", 0, 1) != std::string::npos && tempo.find("/", 0, 1) == std::string::npos && _locationRespond == -1) {
		rootPath();
		setPath((_config->getLocationPath(_locationRespond) + "/" + tempo));
		checkExist();
	}
	if (_locationRespond == -1) {
		std::vector<std::string>	requesty;
		std::vector<std::string>	locationy;
		std::string					pathForSet;
		i = 0;
		requesty = slashSplit(temp.request_target);
		while(i < _config->getLocationSize()) {
			locationy = slashSplit(_config->getLocationPath(i));
			pathForSet = pathCompare(requesty, locationy);
			if (!pathForSet.empty()) {
				setPath(pathForSet);
				if (_locationRespond == -1) {
					_locationRespond = i;
				}
				break;
			}
			i++;
		}
	}
	if (_locationRespond == -1) {
		setCode(404);
	}
	else {
		checkExist();
	}
	addLog();
}

void		Response::set_Allow_to_Header()
{
	std::string tmp;

	if (_config->getLocationGet(_locationRespond))
		tmp += "GET, ";
	if (_config->getLocationHead(_locationRespond))
		tmp += "HEAD, ";
	if (_config->getLocationPost(_locationRespond))
		tmp += "POST, ";
	if (_config->getLocationPut(_locationRespond))
		tmp += "PUT, ";
	_header["Allow"] = tmp.substr(0, tmp.rfind(","));
}

void		Response::check_error(const std::string &error_msg, Request &request)
{
    (void)(request);
    utils::log(*_config, __FILE__, "check_error");
    if (_code >= 400)
    {
		if (_code == 405 || _code == 501)
			set_Allow_to_Header();
		if (_code == 201)
			_header["Location"] = getPath();
		_body_size = error_msg.length();
		setContentType("html");
	}
}

void		Response::check_syntax(Request &request)
{
    utils::log(*_config, __FILE__, "check_syntax");
    if (!request.isMethodValid())
    {
        setCode(400); 
        return;
    }
    if (request.getStartLine().request_target.size() > 8000)
    {
        setCode(501); 
        return;
    }
    if (!request.isRequestTargetValid())
    {
        setCode(400); 
        return;
    }
    if (!request.isHttpVersionValid())
    {
        setCode(505); 
        return;
    }
}

void		Response::check_auth(Request &request)
{
    (void)(request);
    std::string path = getPath();
    size_t len = getTargetFile().first.length() + getTargetFile().second.length();
    std::string path_to_htpasswd(path.begin(), path.end() - len - 1);
    path_to_htpasswd += ".htpasswd";
    if (utils::file_exists(path_to_htpasswd))
    {
        std::map<std::string, std::string> header = request.getHeader();
        if (header.find("Authorization") == header.end())
        {
            utils::log(*_config, __FILE__, "Authorization not found");
            setCode(401);
            _header["WWW-Authenticate"] = "Basic realm=\"simple\"";
        }
        else
        {
            std::vector<char> htpasswd(utils::read_file(path_to_htpasswd));
            std::string htpasswd_str(htpasswd.begin(), htpasswd.end());
            std::string auth_line = header["Authorization"].substr(6);
            utils::log(*_config, __FILE__, "Authorization found, htpasswd_str: "
                    + htpasswd_str + ", auth_line decoded: "
                    + utils::base64decode(auth_line));
            if (htpasswd_str != utils::base64decode(auth_line))
            {
                setCode(401);
            }
        }
    }
}

void		Response::check_method(Request &request)
{
    utils::log(*_config, __FILE__, "check_method");
    std::string method = request.getStartLine().method;
	ProcessMethod process;

	process.secretary_Request(request, *this, _config, method);
}

void		Response::check_accept(Request &request)
{
    utils::log(*_config, __FILE__, "check_accept");
	
	if (request.getHeader().count("Accept-Language"))
		if (request.getHeader()["Accept-Language"].find("ru") == std::string::npos && request.getHeader()["Accept-Language"].find("en") == std::string::npos)
			setCode(406);

	if (request.getHeader().count("Accept-Charset"))
		if (request.getHeader()["Accept-Charset"].find("utf-8") == std::string::npos)
			setCode(406);
}

void		Response::setCode(int code)
{
	_code = code;
}

void	Response::setBody(const std::string &body)
{
	_body = body;
	setBodySize(body.length());
}

void	Response::setBodySize(size_t len)
{
	_body_size = len;
}

void	Response::setPath(std::string path)
{
	_real_path = path;
}

int get_day_of_week(tm &timeinfo) 
{
	int last_number = timeinfo.tm_year % 100;
	int year_code = (6 + last_number + last_number / 4) % 7;
	int month = timeinfo.tm_mon + 1;
	int month_code;
	if (month == 1 || month == 10)		month_code = 1;
	else if (month == 5) 				month_code = 2;
	else if (month == 8)				month_code = 3;
	else if (month == 6)				month_code = 5;
	else if (month == 12 || month == 9)	month_code = 6;
	else if (month == 7 || month == 4)	month_code = 0;
	else								month_code = 4;
	int day_of_week = (timeinfo.tm_mday + month_code + year_code) % 7;
	if (day_of_week == 0 || day_of_week == 1)
		day_of_week = (day_of_week == 0) ? 6 : 0;
	return day_of_week;
}

void get_date(tm &timeinfo, long time) 
{
	int year = 70;
	int number_month = 1;
	timeinfo.tm_min = time % 3600 / 60;
	timeinfo.tm_sec = time % 3600 % 60;
	time /= 3600;
	timeinfo.tm_hour = time % 24 + 3;
	if (timeinfo.tm_hour >= 24) { timeinfo.tm_hour -= 24; ++timeinfo.tm_mday; }
	time = (time - (timeinfo.tm_hour - 3)) / 24 + 1;
	for (; time >= 365; ++year)
		time -= (year % 4 == 0) ? 366 : 365;
	timeinfo.tm_year = year;
	bool leap = timeinfo.tm_year % 4 == 0;
	for (; time >= 28 || (time >= 29 && leap); ++number_month)
		if (number_month == 2)
			time -= (leap) ? 29 : 28;
		else if (number_month <= 8)
			time -= (number_month % 2 != 0) ? 31 : 30;
		else
			time -= (number_month % 2 != 0) ? 30 : 31;
	timeinfo.tm_mon = number_month - 1;
	timeinfo.tm_mday = time;
	timeinfo.tm_wday = get_day_of_week(timeinfo);
}

std::string Response::get_time() 
{
	struct timeval time;
	struct tm timeinfo;
	char buff[100];

	gettimeofday(&time, NULL);
	get_date(timeinfo, time.tv_sec + (time.tv_usec / 1000000));
	strftime(buff, 100, "%a,  %d %b %Y %X GTM", &timeinfo);
	return std::string(buff);
}

void		Response::addHeader(Request &request, std::string &headers)
{
    utils::log(*_config, __FILE__, "addHeader");
    _start_line.http_version = HTTP;
	_start_line.code = toString(_code);
	_start_line.message = getMessage(_code);
	_header["Date"] = get_time();
	_header["Server"] = _config->getServerName();
	_header["Content-Length"] = toString(_body_size);
	setContentType(_target_file.second);
	_header["Content-Language"] = "en-US, ru-RU";
	/* _header["Connection"] = "close"; */
	_header["Content-Location"] = request.getStartLine().request_target;

	std::map < std::string, std::string >::iterator beg = _header.begin();
	std::map < std::string, std::string >::iterator end = _header.end();

	headers.append(
			_start_line.http_version + SPACE
			+ _start_line.code + SPACE
			+ _start_line.message + CRLF);

	while (beg != end)
	{
		headers.append(beg->first + ":" + SPACE);
		headers.append(beg->second + CRLF);
		++beg;
	}
}

std::pair<std::string, std::string>  Response::getTargetFile() const
{
	return (_target_file);
}

void Response::setTargetFile()
{
	std::string name;
	size_t dot_pos;
	size_t slash_pos;

	dot_pos = _real_path.rfind('.');
	slash_pos = _real_path.rfind('/');
	if (dot_pos != std::string::npos)
	{
		_target_file.first = _real_path.substr(slash_pos + 1, dot_pos - slash_pos - 1);
		_target_file.second = _real_path.substr(dot_pos + 1);
	}

}

int	Response::getLocationRespond()
{
	return (_locationRespond);
}

void Response::setContentType(std::string type) 
{
	if (type == "txt")
		_header["Content-Type"] = "text/plain;charset=utf-8";
	else if (type == "html")
		_header["Content-Type"] = "text/html;charset=utf-8";
	else if (type == "jpg")
		_header["Content-Type"] = "image/jpeg;";
	else if (type == "gif")
		_header["Content-Type"] = "image/gif;";
	else if (type == "png")
		_header["Content-Type"] = "image/png;";
	else if (type == "ico")
		_header["Content-Type"] = "image/vnd.microsoft.icon;";
	else
		_header["Content-Type"] = "application/octet-stream";
}

void		Response::addBody(const std::string &error_msg)
{	
    utils::log(*_config, __FILE__, "addBody");
    if (_code < 400)
    {
		/* std::cout << "проверка!" << _body << std::endl; */

        _buf.insert(_buf.end(), _body.begin(), _body.end());
		/* std::string::iterator beg = _body.begin(); */
		/* std::string::iterator end = _body.end(); */
		
		/* while (beg != end) */
		/* { */
		/* 	_buf.push_back(*beg); */
		/* 	++beg; */
		/* } */
	}
	else
	{
		std::string::const_iterator beg = error_msg.begin();
		std::string::const_iterator end = error_msg.end();

		while (beg != end)
		{
			_buf.push_back(*beg);
			++beg;
		}
	}
}

void        Response::clear()
{
    _code = 0;
    _start_line.code = "";
    _start_line.message = "";
    _start_line.http_version = "";
    _header.clear();
    /* Clear memory used by _buf, using clear() method insufficient */
    /* https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Clear-and-minimize */
    std::vector<char>().swap(_buf);
    std::string().swap(_body);
	_body_size = 0;
	_header_size = 0;
    _real_path = "";
}
