#pragma once
#include <cstddef>
#include <fstream>
#include <string>
#include "struct.hpp"
#include "Location.hpp"
#include <iostream>
#include <sstream>

enum {
	WHOLE,
	PART,
	NONE
};

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
	std::string			_tempFilePath;
	int					_tempFileFd;
	int					_errorCode;
	size_t				_bodyLen;
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
	Request(const Request &inst);
	~Request();

	// Operator Overload
	Request& operator=(const Request &rhs);

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
	ssize_t const &getBodyLen() const;
	void setBody(std::string &body);
	bool isBodyValid() const;
	bool isNeedAnswer();
	void setAddedIndex(bool index);
	void setFilePath(std::string &path);
	int generateTempFile();
	void parseFileName();
	bool isHeaderDone() const;
	void addData(char **buffer, size_t const &nbytes);
	int isValid() const;
	void ParseBodyHeader(char **buffer);
};
