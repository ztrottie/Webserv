#pragma once
#include <map>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include "Server.hpp"
#include "struct.hpp"

class Webserv
{
private:
	int								_kq;
	int								_nbServer;
	int								_nbClients;
	std::map<int, serverInfo*>		_clientMap;
public:
	// Constructors / Destructor
	Webserv();
	Webserv(const Webserv &inst);
	~Webserv();

	// Operator Overload
	Webserv& operator=(const Webserv &rhs);

	// Functions
	void addNewServer(uint16_t port, const char *host, std::string name);
	void acceptConnection(serverInfo *info);
	void loop();
};

// Webserv_HPP