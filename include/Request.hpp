#pragma once
#include <string>
#include "struct.hpp"

class Request
{
private:
	std::string _raw;
	std::string	_method;
	std::string _uri;
	std::string _filePath;
	std::string _extraPath;
	std::string _stringQuerry;
	std::string _host;
	std::string _port;
	std::string _type;
	std::string _boundary;
	std::string _clientAddr;
	std::string _clientBody;

	//Functions
	std::string _search(std::string const &searching, char endChar);
	void _uriParser();
	void _setHostPort();
	void _requestBodyParser();

public:
	// Constructors / Destructor
	Request(std::string const &received, socketInfo *client);
	Request(const Request &inst);
	~Request();

	// Operator Overload
	Request& operator=(const Request &rhs);

	// Functions
	std::string const &getMethod() const;
};
