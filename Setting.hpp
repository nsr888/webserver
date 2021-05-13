#ifndef SETTING_HPP
# define SETTING_HPP
# include <string>
# include <vector>

struct CGI {
	std::string					_fileType;
	std::string					_path;
};

struct Location {
	std::string					_path;
	std::string					_locationName;
	std::string					_file;
	std::string					_ErrorPage;
	int							_get;
	int							_put;
	int							_post;
	int							_head;
	int							_autoindex;
	int							_maxBodySizeMB;
};

class Setting {
private:
	std::string					_server_name;
	std::string					_host;
	int							_port;
	std::vector<Location>		_location;
	std::vector<CGI>			_CGI;
	int							_debug;
	

public:
	Setting(void);
	~Setting();
	Setting(const Setting & other);
	Setting & operator=(const Setting & other);

	/* Setters */
	void						setHost(std::string host);
	void						setPort(int port);
	void						setServerName(std::string server_name);
	void						setDebugLevel(int debug);
	void						setMaxBodySize(int BodySize);
	void						pushNewLocation(Location temp);
	void						pushNewCGI(CGI temp);
 
	/* Getters */
	std::string& 				getHost() { return _host; }
	int							getPort() { return _port; }
	std::string& 				getServerName() { return _server_name; }
	int							getDebugLevel() { return _debug; }

    std::string&				getLocationPath(int i) { return _location[i]._path; }
    std::string&				getLocationName(int i) { return _location[i]._locationName; }
    std::string&				getLocationFile(int i) { return _location[i]._file; }
    int							getLocationPut(int i) { return _location[i]._put; }
    int							getLocationPost(int i) { return _location[i]._post; }
    int							getLocationGet(int i) { return _location[i]._get; }
    int							getLocationHead(int i) { return _location[i]._head; }
    int							getLocationAutoindex(int i) { return _location[i]._autoindex; }
    int							getLocationSize() { return _location.size(); }
	int							getLocationMaxBodySize(int i) { return _location[i]._maxBodySizeMB; }

	
	std::string&				getCGIType(int i) { return _CGI[i]._fileType; }
	std::string&				getCGIPath(int i) { return _CGI[i]._path; }
	int							getCGISize() { return _CGI.size(); }
};

#endif
