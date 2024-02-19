#include "../include/Response.hpp"
#include "../include/struct.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

Response::Response() {}

Response::Response(const Response &inst) {
	std::cout << "Copy Response constructor" << std::endl;
	*this = inst;
}

Response::~Response() {
	std::cout << "Response destructor" << std::endl;
}

Response& Response::operator=(const Response &rhs) {
	std::cout << "Response operator = overload" << std::endl;
	if (this != &rhs) {

	}
	return *this;
}

void	Response::setPath(std::string const & path) {
	_path = path;
}

const std::string & Response::getBody() const {
	return _body;
}

const std::string & Response::getPath() const {
	return _path;
}

int Response::openPath(std::string const &path) {
	std::ifstream file(path, std::ios::binary);

	if (!file.is_open()) {
		return INTERNALSERVERROR;
	}
	std::ostringstream oss;
    oss << file.rdbuf();
    _body = oss.str();
	file.close();
	return (OK);
}

void Response::setBody(std::string const & body) {
	_body = body;
}