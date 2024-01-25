#pragma once
#include "Server.hpp"
#include <arpa/inet.h>

enum eType {
	SERVER,
	CLIENT
};

struct socket_in {
	int		socket;
	Server	*servInst;
	sockaddr_in clientAddress;
	socklen_t clientAddressLength;
	int		type;
};