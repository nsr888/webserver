#ifndef SETTING_HPP
# define SETTING_HPP
# include <string>

class Setting {
private:
    std::string     _server_name;
    std::string     _host;
    std::string     _errorPage;
    int             _port;

public:
    Setting() {};
    ~Setting() {};

    /* Setters */
    void			setHost(std::string& host);
	void			setPort(int port);
	void			setServerName(std::string& server_name);
	void			setErrorPage(std::string& error_page);

    /* Getters */
	std::string& 		getHost() { return _host; }
	int	                getPort() { return _port; }
	std::string& 		getServerName() { return _server_name; }
	std::string&		getErrorPage() { return _errorPage; }
};

#endif
