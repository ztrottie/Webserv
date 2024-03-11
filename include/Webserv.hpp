#pragma once
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include "struct.hpp"
#include "Server.hpp"

#include <csignal>
#include <map>


class Webserv
{
private:
	int								_kq;
	int								_nbServer;
	int								_nbClients;
	std::map<int, socketInfo*>		_clientMap;
public:
	// Constructors / Destructor
	Webserv();
	Webserv(const Webserv &inst);
	~Webserv();

	// Operator Overload
	Webserv& operator=(const Webserv &rhs);

	// Functions
	void addNewServer(uint16_t port, const char *host, std::string name, Router *router);
	void acceptConnection(socketInfo *info);
	void loop();
};

// Webserv_HPP