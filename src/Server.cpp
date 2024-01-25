#include "../include/Server.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <stdexcept>
#include <sys/_endian.h>
#include <sys/socket.h>
#include "../include/color.h"
#include <netinet/in.h>

Server::Server() : _port(8080), _host("10.12.6.9"), _name("default") {
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
	std::cout << _name <<  " is listening on port " << _port << std::endl;
	sockaddr_in clientAddress = {};
	socklen_t clientAddressLength = sizeof(clientAddress);
	int clientSocket = accept(_listSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressLength);
	if (clientSocket == -1) {
            std::cout << "accept failed" << std::endl;
    }
	std::cout << "name" << " accepted connection from " << inet_ntoa(clientAddress.sin_addr) << std::endl;
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
	std::cout << _name <<  " is listening on port " << _port << std::endl;
	sockaddr_in clientAddress = {};
	socklen_t clientAddressLength = sizeof(clientAddress);
	int clientSocket = accept(_listSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressLength);
	if (clientSocket == -1) {
            std::cout << "accept failed" << std::endl;
    }
	std::cout << "name" << " accepted connection from " << inet_ntoa(clientAddress.sin_addr) << std::endl;
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