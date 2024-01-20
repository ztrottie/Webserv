#include "../include/Server.hpp"
#include <iostream>
#include "../include/color.h"

Server::Server() {
	std::cout << "Initializing the default webserv" << std::endl;

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