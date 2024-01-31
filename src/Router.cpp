#include "../include/Router.hpp"

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

int Router::getErrorPage(std::string &path, int errorCode){
	std::map<int, std::string>::const_iterator it = _errorPagesLocation.find(errorCode);
	if (it == _errorPagesLocation.end())
		return 500;
	path = _errorPagesLocation[errorCode];
	return errorCode;
}

int Router::checkAllowedMethod(std::string const &method, Location *loc){
	int perm = loc->isMethodAllowed(method);
	if (perm == 405)
		return METHNOTALLOWED;
	if (perm == NOT_FOUND){
		std::vector<const std::string>::const_iterator it = _allowedMethod.begin();
		for (; it != _allowedMethod.end() && *it != method;++it){}
		if (it == _allowedMethod.end())
			return NOT_FOUND;
	}
	return FOUND;
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
	int methodCode = checkAllowedMethod(method, loc);
	if (methodCode == NOT_FOUND && methodCode == METHNOTALLOWED)
		return getErrorPage(path, methodCode);
	if (loc->getRoot(path) == NOT_FOUND)
		path = _root;
	path += URI;
	for (int i = 0; i < 2;i++){
		int code = checkIfFileIsValid(path);
		if (code == INTERNALSERVERROR)
			return code;
		if (code == 1){
			path += "/" + _index;
			continue;
		}
		else if (code == 2)
			break;
		else if (code == NOTFOUND)
			return code;
	}
	return (OK);
}