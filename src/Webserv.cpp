#include "../include/Webserv.hpp"
#include <iostream>

Webserv::Webserv() {
	std::cout << "Default Webserv constructor " << std::endl;
}

Webserv::Webserv(const Webserv &inst) {
	std::cout << "Copy Webserv constructor " << std::endl;
	*this = inst;
}

Webserv::~Webserv() {
	std::cout << "Webserv destructor" << std::endl;
}

Webserv& Webserv::operator=(const Webserv &rhs) {
	std::cout << "Webserv operator = overload" << std::endl;
	if (this != &rhs) {

	}
	return *this;
}