#pragma once

#include <cstddef>
#include <cstdint>
#include <ctime>
#include <string>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include "struct.hpp"
#include "Router.hpp"
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include "../include/color.h"
#include "../include/utils.hpp"
#include "../include/Response.hpp"

class request;
class Server
{
private:
	int														_listSocket;
	uint16_t	 											_port;
	const char												*_host;
	std::string												_name;
	unsigned int											_clientBodySize;
	Router													*_serverRouter;
	char													*_response;
	size_t													_responseSize;
	char													*_responsePtr;

public:
	// Constructors / Destructor
	Server(uint16_t port, const char *host, std::string name, Router *router, socketInfo *server);
	Server(const Server &inst);
	~Server();

	// Operator Overload
	Server& operator=(const Server &rhs);

	// Functions
	Router *getRouter();
	int sendData(int const &socket);
	int acceptConnection(socketInfo *client);
	int handleClient(socketInfo *client, int type);
	void codeMessage(int code, std::string &message);
	int headerGenerator(int code, std::string &header, std::string const &body, std::string const &contentTypeRaw);
	void contentTypeGenerator(std::string &contentType, std::string const &path);
	int recieveRequest(socketInfo *client);
	int handleRequest(socketInfo *client);
	void getEnv(socketInfo *client, char **envp);
	std::string const &getName() const;
};

// Server_HPP