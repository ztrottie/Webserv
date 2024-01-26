#pragma once

#include <cstdint>
#include <string>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include "struct.hpp"
#include "utils.hpp"
// #include "Router.hpp"

class Server
{
private:
	int														_listSocket;
	uint16_t	 											_port;
	const char												*_host;
	std::string												_name;
	// unsigned int											_clientBodySize;
	// Router												_serverRouter;

public:
	// Constructors / Destructor
	Server(serverInfo *server);
	Server(uint16_t port, const char *host, std::string name, serverInfo *server);
	Server(const Server &inst);
	~Server();

	// Operator Overload
	Server& operator=(const Server &rhs);

	// Functions
	int acceptConnection(serverInfo *client);
};

// Server_HPP