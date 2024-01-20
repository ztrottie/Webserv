#pragma once

#include <utility>
#include <vector>
#include <iostream>

class Server
{
private:
	unsigned short 	_port;
	std::string		_host;
	std::string		_name;
	std::vector<std::pair<unsigned short, std::string> > _pathsErrorPages;
	unsigned int	_clientBodySize;
	

public:
	// Constructors / Destructor
	Server();
	Server(const Server &inst);
	~Server();

	// Operator Overload
	Server& operator=(const Server &rhs);

	// Functions

};

// Server_HPP