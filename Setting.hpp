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
    std::string                             _server_name;
    std::string                             _host;
    int                                     _port;
    std::vector<std::string>                _rootLocation;
    std::vector<std::string>                _errorLocation;
    std::vector<std::string>                _indexLocation;
    std::vector<std::string>                _faviconLocation;
    int                                     _maxBodySizeMB;
    /* private field '_isLastServer' is not used */
    /* int                                     _isLastServer; */
    std::vector<Location>                   _location;

public:
    Setting(void) {};

    ~Setting() {};

    /* Setters */
    void			setHost(std::string host);
	void			setPort(int port);
	void			setServerName(std::string server_name);
    void            setMaxBodySize(int BodySize);

    /* for test only */
    void            pushNewLocation(Location temp);
    void            testfillError(void);
    void            testfillIndex(void);
    void            testfillFavicon(void);
    void            testfillRoot(void);

    /* Getters */
	std::string& 		getHost() { return _host; }
	int	                getPort() { return _port; }
	std::string& 		getServerName() { return _server_name; }
    int	                getMaxBodySize() { return _maxBodySizeMB; }
    std::string&        getLocationPath(int i) { return _location[i]._path; }
    std::string&        getLocationName(int i) { return _location[i]._locationName; }
    std::string&        getLocationFile(int i) { return _location[i]._file; }
    int                 getLocationPut(int i) { return _location[i]._put; }
    int                 getLocationPost(int i) { return _location[i]._post; }
    int                 getLocationGet(int i) { return _location[i]._get; }
    int                 getLocationDelete(int i) { return _location[i]._delete; }

};

#endif
