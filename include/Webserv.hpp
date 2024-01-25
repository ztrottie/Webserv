#pragma once
#include <map>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include "struct.hpp"

class Webserv
{
private:
	int								_kq;
	int								_nbSockets;
	struct kevent					_event;
	std::map<int, socket_in*>		_clientMap;
public:
	// Constructors / Destructor
	Webserv();
	Webserv(const Webserv &inst);
	~Webserv();

	// Operator Overload
	Webserv& operator=(const Webserv &rhs);

	// Functions
	void addNewServer();
};

// Webserv_HPP