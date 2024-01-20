#pragma once

#include <cstdint>
#include <utility>
#include <vector>
#include <string>
// #include "Router.hpp"

class Server
{
private:
	int														_listSocket;
	uint16_t	 											_port;
	const char												*_host;
	std::string												_name;
	std::vector<std::pair<unsigned short, std::string> >	_pathsErrorPages;
	// unsigned int											_clientBodySize;
	// Router													_serverRouter;

public:
	// Constructors / Destructor
	Server();
	Server(uint16_t port, const char *host, std::string name);
	Server(const Server &inst);
	~Server();

	// Operator Overload
	Server& operator=(const Server &rhs);

	// Functions
	int getListSocket() const;
};

// Server_HPP