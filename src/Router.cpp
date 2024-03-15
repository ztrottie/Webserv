#include "../include/Router.hpp"
#include "../include/utils.hpp"
#include <cstddef>

Router::Router() {
	std::cout << timestamp() << " Initializing the server Router!" << std::endl;
	_clientMaxBodySize = -1;
}

Router::~Router() {
	std::cout << "Router destructor" << std::endl;
}

void Router::setRoot(std::string const &root){
	_root = root;
}

void Router::setIndex(std::string const &index){
	_index = index;
}

void Router::addAllowedMethod(std::string const &method){
	_allowedMethod.push_back(method);
}

void Router::addErrorPage(const int errorNumber, std::string pathToError) {
	_errorPagesLocation.insert(std::make_pair(errorNumber, pathToError));
}

void Router::addLocation(std::string const &key, Location *loc){
	_locations.insert(std::make_pair(key, loc));
}

size_t Router::getClientMaxBodySize(Location *location) const{
	size_t value = _clientMaxBodySize;
	location->getClientMaxBodySize(value);
	return value;
}

long long Router::getClientMaxBodySizeParsing() const{
	return (_clientMaxBodySize);
}

int Router::isContentLengthValid(Location *location, size_t const &bodyLen) {
	if (getClientMaxBodySize(location) < bodyLen)
		return (TOOLARGE);
	return (OK);
}

void Router::setClientMaxBodySize(size_t value){
	_clientMaxBodySize = value;
}

void Router::parseUri(std::string &cpy){
	for (std::map<std::string, Location*>::const_iterator it = _locations.end(); it == _locations.end();){
		it = _locations.find(cpy);
		if (it == _locations.end()){
			trimURI(cpy);
		}
	}
}

void Router::trimURI(std::string &uri){
	std::cout << "timURI: " << uri << std::endl;
	size_t index = uri.rfind('/');
	if (index == 0){
		uri = '/';
		return ;
	}
	uri = uri.substr(0, index);
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
	return INTERNALSERVERROR;
}

int Router::getErrorPage(std::string &path, int errorCode, Location *loc){
	std::map<int, std::string>::const_iterator it = _errorPagesLocation.find(errorCode);
	if (!loc || loc->isErrorCodeValid(errorCode, path) == NOTFOUND){
		if (it == _errorPagesLocation.end())
			return INTERNALSERVERROR;
		path = _errorPagesLocation[errorCode];	
	}
	return OK;
}

int Router::getLocation(Request *request, Location *&loc, std::string &fullPath) {
	fullPath = request->getFilePath();
	parseUri(fullPath);
	std::map<std::string, Location*>::const_iterator it = _locations.find(fullPath);
	if (it != _locations.end()) {
		loc = _locations[fullPath];
		return loc->isMethodAllowed(request->getMethod());
	}
	else
		return INTERNALSERVERROR;
}

int Router::getFile(Request *request, std::string &path) {
	std::string uriCopy = request->getFullPath();
	if (request->getLocation()->getRoot(uriCopy) == NOTFOUND)
		uriCopy = _root;
	uriCopy += request->getFilePath();
	for (int i = 0; i < 2;i++){
		int code = checkIfFileIsValid(uriCopy);
		if (code == INTERNALSERVERROR)
			return code;
		if (code == IS_DIR && request->getMethod() == "GET"){
			if (uriCopy.back() != '/')
				uriCopy += "/";
			uriCopy += _index;
			continue;
		}
		else if (code == IS_FILE)
			break;
		else if (code == NOTFOUND)
			return code;
	}
	path = uriCopy;
	return (OK);
}

std::string	Router::getRoot(Location *loc) const {
	std::string root;
	if (loc && loc->getRoot(root) == FOUND)
		return root;
	return _root;
}

std::string	Router::getIndex() const {
	return _index;
}

std::string Router::getErrorForParsing(int code) const {
	std::map<int, std::string>::const_iterator it = _errorPagesLocation.find(code);
	return it->second;
}

std::string	Router::getLocationParsing() const{
	if (_locations.empty()){
		return "";
	}
	string res = "	";
	for (std::map<std::string, Location*>::const_iterator it = _locations.begin(); it != _locations.end(); it++) {
		res += it->second->getName() + "	";
	}
	return res;
}

std::string	Router::getRootParsing() const{
	return _root;
}
