#pragma once
#include <map>
#include <string>
#include <vector>
#include "struct.hpp"

class Router;

#define FOUND 1
#define NOT_FOUND 0

class Location
{
private:
	const std::string 			_name;
	std::string					_root;
	std::string					_index;
	std::map<int, std::string>	_errorPagesLocation;
	std::string					_rewrite;
	std::vector<std::string> 	_allowedMethod;

public:
	// Constructors / Destructor
	Location();
	Location(std::string const &name);
	Location(const Location &inst);
	~Location();

	// Operator Overload
	Location& operator=(const Location &rhs);

	//Setters
	void setRoot(std::string const &root);
	void setIndex(std::string const &index);
	void addErrorPage(const int errorNumber, std::string pathToError);
	void addAllowedMethod(std::string const &method);

	// Functions
	int isMethodAllowed(std::string const &method);
	int isErrorCodeValid(int const &errorCode, std::string &path);
	int getRoot(std::string &root);
};

// Location_HPP