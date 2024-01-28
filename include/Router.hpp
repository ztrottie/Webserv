#pragma once
#include "struct.hpp"
#include "Location.hpp"
#include <string>
#include <map>

class Server;

class Router
{
private:
	Server								*_server;
	const std::string					_root;
	const std::string					_index;
	std::map<int, std::string>			_errorPagesLocation;
	std::map<std::string, Location*>	_locations;
public:
	// Constructors / Destructor
	Router(Server *server, std::string root, std::string index);
	Router(const Router &inst);
	~Router();

	// Operator Overload
	Router& operator=(const Router &rhs);

	// Functions
	void addErrorPage(const int errorNumber, std::string pathToError);
	int getFile(std::string const &URI, std::string &path);
};
