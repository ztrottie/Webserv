#pragma once
#include <arpa/inet.h>

class Server;

enum socketType {
	CLIENT,
	SERVER
};

enum socketStatus {
	KEEP,
	CLOSE
};

enum socketError {
	OK,
	KO
}

struct serverInfo {
	int socket;
	int type;
	struct sockaddr_in client_address;
	Server	*serverInst;
};