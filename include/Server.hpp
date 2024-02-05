#pragma once

#include <cstdint>
#include <string>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include "struct.hpp"
#include "Router.hpp"

class Server
{
private:
	int														_listSocket;
	uint16_t	 											_port;
	const char												*_host;
	std::string												_name;
	unsigned int											_clientBodySize;
	Router													*_serverRouter;

public:
	// Constructors / Destructor
	Server(uint16_t port, const char *host, std::string name, Router *router, unsigned int const &clientBodySize, socketInfo *server);
	Server(const Server &inst);
	~Server();

	// Operator Overload
	Server& operator=(const Server &rhs);

	// Functions
	int acceptConnection(socketInfo *client);
	int handleClient(socketInfo *client, int type);
	Router *getRouter() const;
	void codeMessage(int code, std::string &message);
	int headerGenerator(int code, std::string const &path, std::string &response);
	int contentTypeGenerator(std::string &contentType, std::string const &path);
	void contentLengthGenerator(std::string &contentLength, std::string const &path);
	void contentGenerator(std::string const &path, std::string &response);
	void internalServerError(std::string &response);
	int recieveRequest(socketInfo *client);
	void	handlePost(socketInfo *client, std::string &data);
	int handleRequest(socketInfo *client);
	int handlePostMethod(socketInfo *client);
	std::string findKeyWord(std::string const &from, std::string const &find);
};

// Server_HPP