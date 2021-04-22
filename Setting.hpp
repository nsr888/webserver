#ifndef SETTING_HPP
# define SETTING_HPP
# include <string>
# include <vector>

class Setting {
private:
    std::string                 _server_name;
    std::string                 _host;
    int                         _port;
    std::vector<std::string>    _rootLocation;
    std::vector<std::string>    _errorLocation;
    std::vector<std::string>    _indexLocation;
    std::vector<std::string>    _faviconLocation;

public:
    Setting(void) {};

    ~Setting() {};

    /* Setters */
    void			setHost(std::string host);
	void			setPort(int port);
	void			setServerName(std::string server_name);

    /* for test only */
    void testfillError(void);
    void testfillIndex(void);
    void testfillFavicon(void);
    void testfillRoot(void);

    /* Getters */
	std::string& 		getHost() { return _host; }
	int	                getPort() { return _port; }
	std::string& 		getServerName() { return _server_name; }
};

#endif
