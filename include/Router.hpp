#pragma once
#include "struct.hpp"
#include "Location.hpp"
#include <array>
#include <string>
#include <map>
#include <vector>

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
	void addLocation(std::string const &key, Location *bobsHouse);
	void addAllowedMethod(std::string const &method);

	// Functions
	int getFile(std::string const &method, std::string const &URI, std::string &path);
	void trimURI(std::string &URI);
	int checkFilePerm(std::string const &path);
	int checkIfFileIsValid(std::string const &path);
	int getError(std::string &path, int errorCode);
};
