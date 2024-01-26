#include "../include/Router.hpp"
#include <iostream>

Router::Router() {
	std::cout << "Default Router constructor " << std::endl;
}

Router::Router(const Router &inst) {
	std::cout << "Copy Router constructor " << std::endl;
	*this = inst;
}

Router::~Router() {
	std::cout << "Router destructor" << std::endl;
}

Router& Router::operator=(const Router &rhs) {
	std::cout << "Router operator = overload" << std::endl;
	if (this != &rhs) {

	}
	return *this;
}