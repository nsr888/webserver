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
    std::vector<std::string>    _rootLocation;
    int                         _maxBodySizeMB;

public:
    Setting(void) {
        _server_name;
        _host;
        _port;
        _rootLocation;
        _errorLocation;
        _indexLocation;
        _faviconLocation;
        _rootLocation;
        _maxBodySizeMB;
    };

    ~Setting() {};

    /* Setters */
    void			setHost(std::string host);
	void			setPort(int port);
	void			setServerName(std::string server_name);
    void            setMaxBodySize(int BodySize);

    /* for test only */
    void Setting::testfillError(void);
    void Setting::testfillIndex(void);
    void Setting::testfillFavicon(void);
    void Setting::testfillRoot(void);

    /* Getters */
	std::string& 		getHost() { return _host; }
	int	                getPort() { return _port; }
	std::string& 		getServerName() { return _server_name; }
    int	                getMaxBodySize() { return _maxBodySizeMB; }
};

#endif
