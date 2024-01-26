#pragma once
#include <arpa/inet.h>

class Server;

enum socketType {
	CLIENT,
	SERVER
};

struct serverInfo {
	int socket;
	int type;
	struct sockaddr_in client_address;
	Server	*serverInst;
};