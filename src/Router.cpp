#include "../include/Router.hpp"
#include "../include/color.h"
#include "../include/utils.hpp"
#include <iostream>
#include <utility>

Router::Router(Server *server, std::string root, std::string index) : _server(server), _root(root), _index(index) {
	std::cout << YELLOW << timestamp() << " Initializing the server Router!" << RESET << std::endl;
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

void Router::addErrorPage(const int errorNumber, std::string pathToError) {
	_errorPagesLocation.insert(std::make_pair(errorNumber, pathToError));
}

int Router::getFile(std::string const &URI, std::string &path) {
	return (0);
}