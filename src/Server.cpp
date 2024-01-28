#include "../include/Server.hpp"
#include "../include/color.h"
#include "../include/utils.hpp"
#include <arpa/inet.h>
#include <iosfwd>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/_endian.h>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <sstream>

Server::Server(uint16_t port, const char *host, std::string name, Router *router, serverInfo *server) : _port(port), _host(host), _name(name) {
	std::cout << YELLOW << timestamp() << " Initializing a Server named " << _name << " on " << _host << ":" << _port << RESET << std::endl;
	server->socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server->socket == -1)
		throw std::invalid_argument("Socket creation Failed");
	fcntl(server->socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(_host);
	serverAddr.sin_port = htons(_port);
	if (bind(server->socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
		throw std::invalid_argument("bind creation Failed");
	listen(server->socket, 5);
	server->type = SERVER;
	_serverRouter = router;
	_listSocket = server->socket;
	std::cout << GREEN << timestamp() << " " << _name <<  " is listening on port " << _port << RESET << std::endl;
}

Server::Server(const Server &inst) {
	std::cout << "Copy Server constructor" << std::endl;
	*this = inst;
}

Server::~Server() {
	std::cout << "Server destructor" << std::endl;
	delete _serverRouter;
}

Server& Server::operator=(const Server &rhs) {
	std::cout << "Server operator = overload" << std::endl;
	if (this != &rhs) {

	}
	return *this;
}

int Server::acceptConnection(serverInfo *client) {
	std::cout << YELLOW << timestamp() << " incomming connection trying to connect to " << _name << RESET << std::endl;
	struct sockaddr_in clientAddress = {};
	socklen_t clientAdressLen = sizeof(client->client_address);
	client->type = CLIENT;
	client->socket = accept(_listSocket, reinterpret_cast<struct sockaddr*>(&clientAddress), &clientAdressLen);
	if (client->socket < 0 || fcntl(client->socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
		std::cout << RED << timestamp() << " Server: "  << _name << " had a problem with connection incomming from: " << inet_ntoa(clientAddress.sin_addr) << " closing connection!" << std::endl;
		return CLOSE;
	}
	client->client_address = clientAddress;
	client->serverInst = this;
	std::cout << GREEN << timestamp() << " incomming connnection from " << inet_ntoa(client->client_address.sin_addr) << " accepted and setted to the socket: " << client->socket << "!" << RESET << std::endl;
	return (KEEP);
}

int Server::handleClient(serverInfo *client) {
	char buffer[1024] = {0};
	
	ssize_t nbytes;
	nbytes = recv(client->socket, buffer, sizeof(buffer), 0);
	std::string data = buffer;
	std::cout << timestamp() << " client sokcet: " << client->socket << std::endl;
	std::cout << timestamp() << " nbytes recv: " << nbytes << std::endl;
	if (nbytes == 0) {
		std::cout << timestamp() << " client closed the connection!" << std::endl;
		return (CLOSE);
    } else if (nbytes == -1) {
		std::cout << timestamp() << " problem while recieving data closing connection" << std::endl;
	}
	std::stringstream ss(data);
	std::string uri;
	std::string method;
	std::getline(ss, method, ' ');
	std::getline(ss, uri, ' ');
	std::cout << uri << std::endl;
	std::string path;
	int result = _serverRouter->getFile(uri, path);
	std::string response = "HTTP/1.1 404 Not Found\r\nContent-Length: 14\r\n\r\nAccess Denied\n";
	send(client->socket, response.c_str(), response.size(), 0);
	std::cout << timestamp() << " Invalid request from the client closing the connection with error 404" << std::endl;
	return (CLOSE);
}

Router *Server::getRouter() const {
	return _serverRouter;
}
