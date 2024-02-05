#pragma once
#include "struct.hpp"
#include "Location.hpp"
#include <array>
#include <string>
#include <map>
#include <vector>
#include "../include/color.h"
#include "../include/utils.hpp"
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>

#define IS_DIR 1
#define IS_FILE 2

class Server;

class Router
{
private:
	std::string							_root;
	std::string							_index;
	std::map<int, std::string>			_errorPagesLocation;
	std::map<std::string, Location*>	_locations;
	std::vector<std::string> 			_allowedMethod;

public:
	// Constructors / Destructor
	Router();
	Router(const Router &inst);
	~Router();

	// Operator Overload
	Router& operator=(const Router &rhs);

	//Setters
	void setRoot(std::string const &root);
	void setIndex(std::string const &index);
	void addErrorPage(const int errorNumber, std::string pathToError);
	void addLocation(std::string const &key, Location *loc);
	void addAllowedMethod(std::string const &method);

	// Functions
	int getFile(Request *request, std::string &path);
	void trimURI(std::string &URI);
	int checkFilePerm(std::string const &path);
	int checkIfFileIsValid(std::string const &path);
	int getErrorPage(std::string &path, int errorCode);
	int checkAllowedMethod(std::string const &method, Location *loc);
};
