#include "../include/Server.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <stdexcept>
#include <sys/_endian.h>
#include <sys/socket.h>
#include "../include/color.h"
#include <netinet/in.h>

Server::Server() : _port(8080), _host("127.0.0.1"), _name("default") {
	std::cout << YELLOW "Initializing a default Server named " << _name << " on " << _host << ":" << _port << RESET << std::endl;
	_listSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_listSocket == -1)
		throw std::invalid_argument("Socket creation Failed");
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(_host);
	serverAddr.sin_port = htons(_port);
	if (bind(_listSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
		throw std::invalid_argument("Socket creation Failed");
	listen(_listSocket, 5);
	std::cout << "socket:" << _listSocket <<  " is listening on port " << _port << std::endl;
}

Server::Server(uint16_t port, const char *host, std::string name) : _port(port), _host(host), _name(name) {
	std::cout << YELLOW "Initializing a default Server named " << _name << " on " << _host << ":" << _port << RESET << std::endl;
	_listSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_listSocket == -1)
		throw std::invalid_argument("Socket creation Failed");
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(_host);
	serverAddr.sin_port = htons(_port);
	if (bind(_listSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
		throw std::invalid_argument("Socket creation Failed");
	listen(_listSocket, 5);
	std::cout << "socket:" << _listSocket <<  " is listening on port " << _port << std::endl;
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

int Server::getListSocket() const {
	return _listSocket;
}