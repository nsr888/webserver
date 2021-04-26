#ifndef SETTING_HPP
# define SETTING_HPP
# include <string>
# include <vector>

struct Location {
    std::string                             _path;
    std::string                             _locationName;
    std::string                             _file;
    int                                     _get;
    int                                     _put;
    int                                     _post;
    int                                     _delete;
};

class Setting {
private:
<<<<<<< Updated upstream
    std::string                 _server_name;
    std::string                 _host;
    int                         _port;
    std::vector<std::string>    _rootLocation;
    std::vector<std::string>    _errorLocation;
    std::vector<std::string>    _indexLocation;
    std::vector<std::string>    _faviconLocation;
    std::vector<std::string>    _rootLocation;
    int                         _maxBodySizeMB;
=======
    std::string                             _server_name;
    std::string                             _host;
    int                                     _port;
    std::vector<std::string>                _rootLocation;
    std::vector<std::string>                _errorLocation;
    std::vector<std::string>                _indexLocation;
    std::vector<std::string>                _faviconLocation;
    int                                     _maxBodySizeMB;
    int                                     _isLastServer;
    std::vector<Location>                   _location;

>>>>>>> Stashed changes

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
<<<<<<< Updated upstream
=======
        _location;
        _isLastServer;
>>>>>>> Stashed changes
    };

    ~Setting() {};

    /* Setters */
    void			setHost(std::string host);
	void			setPort(int port);
	void			setServerName(std::string server_name);
    void            setMaxBodySize(int BodySize);
<<<<<<< Updated upstream

    /* for test only */
    void Setting::testfillError(void);
    void Setting::testfillIndex(void);
    void Setting::testfillFavicon(void);
    void Setting::testfillRoot(void);
=======
    void            pushNewLocation(Location temp);

    /* for test only */
    void            testfillError(void);
    void            testfillIndex(void);
    void            testfillFavicon(void);
    void            testfillRoot(void);
>>>>>>> Stashed changes

    /* Getters */
	std::string& 		getHost() { return _host; }
	int	                getPort() { return _port; }
	std::string& 		getServerName() { return _server_name; }
    int	                getMaxBodySize() { return _maxBodySizeMB; }
<<<<<<< Updated upstream
=======
    std::string&        getLocationPath(int i) { return _location[i]._path; }
    std::string&        getLocationName(int i) { return _location[i]._locationName; }
    std::string&        getLocationFile(int i) { return _location[i]._file; }
    int                 getLocationPut(int i) { return _location[i]._put; }
    int                 getLocationPost(int i) { return _location[i]._post; }
    int                 getLocationGet(int i) { return _location[i]._get; }
    int                 getLocationDelete(int i) { return _location[i]._delete; }

>>>>>>> Stashed changes
};

#endif
