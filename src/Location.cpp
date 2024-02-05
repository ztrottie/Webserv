#include "../include/Location.hpp"
#include <iostream>
#include <map>
#include <utility>
#include <vector>

Location::Location() {
	std::cout << "Default Location constructor " << std::endl;
}

Location::Location(std::string const &name) : _name(name) {

}

Location::Location(const Location &inst) {
	std::cout << "Copy Location constructor " << std::endl;
	*this = inst;
}

Location::~Location() {
	std::cout << "Location destructor" << std::endl;
}

Location& Location::operator=(const Location &rhs) {
	std::cout << "Location operator = overide" << std::endl;
	if (this != &rhs) {

	}
	return *this;
}

int Location::isMethodAllowed(std::string const &method){
	if (_allowedMethod.size() == 0)
		return NOT_FOUND;
	std::vector<const std::string>::const_iterator it = _allowedMethod.begin();
	for (; it != _allowedMethod.end() && *it != method;++it){}
	if (it == _allowedMethod.end())
		return FORBIDDEN;
	return FOUND;
}

int Location::isErrorCodeValid(int const &errorCode, std::string &path){
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

void Location::setRoot(std::string const &root) {
	_root = root;
}

void Location::setIndex(std::string const &index) {
	_index = index;
}

void Location::addErrorPage(const int errorNumber, std::string pathToError) {
	_errorPagesLocation.insert(std::make_pair(errorNumber, pathToError));
}

void Location::addAllowedMethod(std::string const &method) {
	_allowedMethod.push_back(method);
}
