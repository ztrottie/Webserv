#pragma once
#include <cstddef>
#include <map>
#include <iostream>
#include <string>
#include <vector>
#include "struct.hpp"

class Router;

#define FOUND 1
#define NOT_FOUND 0

class Location
{
private:
	const std::string 					_name;
	std::string							_root;
	std::string							_index;
	std::map<int, std::string>			_errorPagesLocation;
	std::string							_rewrite;
	std::vector<std::string> 			_allowedMethod;
	long long						_clientMaxBodySize;
	bool								_autoIndex;
	bool								_uploadEnable;
	std::string					_uploadStore;
	bool								_redirection;
	int 								_redirectionCode;
	std::string							_redirectionLocation;

public:
	// Constructors / Destructor
	Location();
	Location(std::string const &name);
	Location(const Location &inst);
	~Location();

	// Operator Overload
	Location& operator=(const Location &rhs);

	//Setters
	void setUploadEnable(bool enable);
	void setRoot(std::string const &root);
	void setIndex(std::string const &index);
	void setAutoIndex(bool const &autoIndex);
	void addErrorPage(const int errorNumber, std::string pathToError);
	void addAllowedMethod(std::string const &method);
	void setUploadStore(std::string const &value);
	void setRedirection(bool value);
	void setRedirectionCode(int code);
	void setRedirectionLocation(std::string str);

	// Functions
	int isMethodAllowed(std::string const &method);
	int isErrorCodeValid(int const &errorCode, std::string &path);
	int getRoot(std::string &root);
	bool getUploadEnable() const;
	bool getAutoIndex() const;
	const std::string	&getUploadStore() const;
	const std::string	&getName() const;

	bool getRedirection() const;
	int getRedirectionCode() const;
	std::string getRedirectionLocation() const;
	void setClientMaxBodySize(long long int value);
	long long getClientMaxBodySize() const;


};

// Location_HPP