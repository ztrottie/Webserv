#include "../include/Server.hpp"
#include <algorithm>
#include <arpa/inet.h>
#include <cstring>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/_endian.h>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>
#include "../include/color.h"
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

Server::Server(uint16_t port, const char *host, std::string name, serverInfo *server) : _port(port), _host(host), _name(name) {
	std::cout << YELLOW << timestamp() << " Initializing a default Server named " << _name << " on " << _host << ":" << _port << RESET << std::endl;
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
	_listSocket = server->socket;
	std::cout << GREEN << timestamp() << " " << _name <<  " is listening on port " << _port << RESET << std::endl;
}

Server::Server(const Server &inst) {
	std::cout << "Copy Server constructor" << std::endl;
	*this = inst;
}

Server::~Server() {
	std::cout << "Server destructor" << std::endl;
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
	fcntl(client->socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	client->client_address = clientAddress;
	client->serverInst = this;
	std::cout << GREEN << timestamp() << " incomming connnection from " << inet_ntoa(client->client_address.sin_addr) << " accepted" << RESET << std::endl;
	return (0);
}

int Server::handleClient(serverInfo *client) {
	char buffer[1024];
	std::string data;
	size_t nbytes = 1024;
	while (nbytes == 1024) {
		std::memset(buffer, 0, sizeof(buffer));
		nbytes = recv(client->socket, buffer, sizeof(buffer), 0);
		data += buffer;
	}
	std::cout << data << std::endl;
	if (nbytes <= 0) {
		std::cout << RED << timestamp() << " server: " << _name << " connection from: " << inet_ntoa(client->client_address.sin_addr) << " CLOSED!!!" << RESET << std::endl;
		return CLOSE;
	}
	if (data.find("GET / HTTP/1.1") != std::string::npos) {
		std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";
        send(client->socket, response.c_str(), response.size(), 0);
	} else {
		return CLOSE;
	}

	return KEEP;
}
