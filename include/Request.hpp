#pragma once
#include <cstddef>
#include <string>
#include "struct.hpp"
#include "Location.hpp"

class Request
{
private:
	socketInfo			*_client;
	Server				*_server;
	Location			*_location;
	std::string	 		_raw;
	std::string			_method;
	std::string 		_uri;
	std::string 		_filePath;
	std::string			_fullPath;
	std::string 		_extraPath;
	std::string 		_stringQuerry;
	std::string 		_host;
	std::string 		_port;
	std::string 		_type;
	std::string 		_boundary;
	std::string 		_serverName;
	std::string 		_clientAddr;
	std::string 		_fileName;
	std::string			_bodyName;
	std::string			_bodyContentType;
	std::string			_bodyContentDispo;
	std::string			_tempFilePath;
	std::string			_extension;
	int					_tempFileFd;
	int					_errorCode;
	size_t				_bodyLen;
	size_t				_bodyLenModified;
	size_t				_nbytesRead;
	size_t				_bodyNbytes;
	size_t				_rawSize;
	bool				_headerDone;
	bool				_bodyStarted;
	bool				_bodyEnded;
	size_t				_bodyLenWritten;


	//Functions
	bool _search(std::string const &searching, char endChar, std::string &result);
	void _uriParser();
	void _setHostPort();
	void _requestBodyParser();
	void _headerParser(char **buffer);
	int _findMostOfEndBoundary(size_t & pos, std::string const & string, size_t & length);

public:
	// Constructors / Destructor
	Request(socketInfo *client, Server *server);
	~Request();

	// Functions
	std::string const &getMethod() const;
	std::string const &getRaw() const;
	std::string const &getUri() const;
	std::string const &getFilePath() const;
	std::string const &getExtraPath() const;
	std::string const &getStringQuerry() const;
	std::string const &getHost() const;
	std::string const &getPort() const;
	std::string const &getType() const;
	std::string const &getBoundary() const;
	std::string const &getClientAddress() const;
	std::string const &getClientBody() const;
	std::string const &getFileContent() const;
	std::string const &getFileName() const;
	std::string const &getServerName() const;
	std::string const &getFullPath() const;
	std::string const &getTempFilePath() const;
	std::string const &getExtension() const;
	std::string const &getBodyContentType() const;
	std::string const &getBodyContentDispo() const;
	std::string const &getBodyName() const;
	Location *getLocation();
	Router *getRouter();
	int const &getErrorCode() const;

	size_t const &getBodyLen() const;
	void addBody(char **buffer);
	bool isBodyValid() const;
	bool isNeedAnswer();
	void setAddedIndex(bool index);
	bool getAddedIndex() const;
	void setFilePath(std::string &path);
	int generateTempFile(std::string &tempFilePath, int &tempFileFd);
	void parseFileName();
	bool isHeaderDone() const;
	void addData(char **buffer, size_t const &nbytes);
	int isValid() const;
	void ParseBodyHeader(char **buffer);
	bool isCgi() const;
};