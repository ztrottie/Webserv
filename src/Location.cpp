#include "../include/Location.hpp"
#include <iostream>
#include <map>
#include <vector>

Location::Location() {
	std::cout << "Default Location constructor " << std::endl;
}

Location::Location(const Location &inst) {
	std::cout << "Copy Location constructor " << std::endl;
}

Location::~Location() {
	std::cout << "Location destructor" << std::endl;
}

Location& Location::operator=(const Location &rhs) {
	std::cout << "Location operator = overide" << std::endl;
	return *this;
}

int Location::getPerm(std::string const &method){
	std::vector<const std::string>::const_iterator it = _allowedMethod->begin();
	for (; it != _allowedMethod->end() && *it != method;++it){}
	if (it == _allowedMethod->end())
		return 405;
	return 1;
}

int Location::getErrorCode(int const &errorCode, std::string &path){
	std::map<int, std::string>::const_iterator it = _errorPagesLocation.find(errorCode);
	if (it == _errorPagesLocation.end())
		return NOT_FOUND;
	path = _errorPagesLocation[errorCode];
	return FOUND;
}

int Location::getRoot(std::string &root){
	if (_root.empty())
		return NOT_FOUND;
	root = _root;
	return FOUND;
}
