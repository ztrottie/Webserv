#include "../include/Router.hpp"
#include "../include/color.h"
#include "../include/utils.hpp"
#include <iostream>
#include <map>
#include <sys/_types/_size_t.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <unistd.h>
#include <utility>

Router::Router(){
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
	return *this;
}

void Router::addErrorPage(const int errorNumber, std::string pathToError) {
	_errorPagesLocation.insert(std::make_pair(errorNumber, pathToError));
}

void Router::trimURI(std::string &uri){
	std::cout << uri << std::endl;
	size_t index = uri.rfind('/');
	if (index == 0){
		uri = '/';
		return ;
	}
	uri = uri.substr(0, index);
}

void Router::addLocation(std::string const &key, Location *loc){
	_locations.insert(std::make_pair(key, loc));
}

int Router::checkIfFileIsValid(std::string const &path){
	struct stat fileStat;

	if (access(path.c_str(), F_OK) != 0)
		return 404;
	if (stat(path.c_str(), &fileStat) == 0){
		if (S_ISDIR(fileStat.st_mode)) 
			return IS_DIR;
		if (S_ISREG(fileStat.st_mode))
			return IS_FILE;
	}
	return 500;
}

int Router::getError(std::string &path, int errorCode){
	std::map<int, std::string>::const_iterator it = _errorPagesLocation.find(errorCode);
	if (it == _errorPagesLocation.end())
		return 500;
	path = _errorPagesLocation[errorCode];
	return errorCode;
}

int Router::getFile(std::string const &method, std::string const &URI, std::string &path) {
	std::string uriCopy = URI;
	for (std::map<std::string, Location*>::const_iterator it = _locations.end(); it == _locations.end();){
		it = _locations.find(uriCopy);
		if (it == _locations.end()){
			trimURI(uriCopy);	
		}
	}
	Location *loc = _locations[uriCopy];
	if (loc->isMethodAllowed(method) == 405){//do this for all error codes *needs to change all below*
		if (loc->isErrorCodeValid(405, path))
			return 405;
		return getError(path, 405);
	}
	if (loc->getRoot(path) == NOT_FOUND)
		path = _root;
	path += URI;
	for (int i = 0; i < 2;i++){
		int code = checkIfFileIsValid(path);
		if (code == 500)
			return 500;
		if (code == 1){
			path += "/" + _index;
			continue;
		}
		else if (code == 2)
			break;
		else if (code == 404)
			return 404;
	}
	//std::cout << uriCopy << std::endl;
	return (200);
}