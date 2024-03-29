#pragma once
#include "struct.hpp"
#include "Location.hpp"
#include "Request.hpp"

#include <sys/stat.h>
#include <unistd.h>

#define IS_DIR 1
#define IS_FILE 2

class Server;
class Response;

class Router
{
private:
	std::string							_root;
	std::string							_index;
	long long							_clientMaxBodySize;
	std::map<int, std::string>			_errorPagesLocation;
	std::map<std::string, Location*>	_locations;
	std::vector<std::string> 			_allowedMethod;

public:
	Router();
	~Router();

	void setRoot(std::string const &root);
	void setIndex(std::string const &index);
	void setClientMaxBodySize(size_t value);
	void addErrorPage(const int errorNumber, std::string pathToError);
	void addLocation(std::string const &key, Location *loc);
	void addAllowedMethod(std::string const &method);

	int getFile(Request *request, std::string &path);
	void trimURI(std::string &URI);
	void parseUri(std::string &cpy);
	int checkFilePerm(std::string const &path);
	int checkIfFileIsValid(std::string const &path);
	int checkAllowedMethod(std::string const &method, Location *loc);
	int checkIfCanExec(std::string const &path);
	int openFile(Request *request);
	int getFileMethod(std::string &path, Request *request);
	bool checkRedirectionCode(int code);
	void checkBodySize(Request *request, int &errorCode);
	int getLocation(Request *request, Location *&loc, std::string &fullPath);
	int isContentLengthValid(Location *location, size_t const &bodyLen);
	int getErrorPage(std::string &path, int errorCode, Location *loc);

	long long	getClientMaxBodySizeParsing() const;
	size_t 		getClientMaxBodySize(Location *location) const;
	std::string	getRoot(Location *loc) const;
	std::string	getIndex() const;
	std::string getErrorForParsing(int code) const;
	std::string getLocationParsing() const;
	std::string	getRootParsing() const;
	Location *getLocationForTesting() const;
};
