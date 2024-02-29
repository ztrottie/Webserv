#include "../include/Router.hpp"
#include "../include/utils.hpp"
#include "../include/Response.hpp"

Router::Router():_clientMaxBodySize(-1){
	std::cout << timestamp() << " Initializing the server Router!" << std::endl;
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

long long int Router::getClientMaxBodySize() const{
	return _clientMaxBodySize;
}

void Router::setClientMaxBodySize(unsigned int value){
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
	std::cout << uri << std::endl;
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
	if (!loc || !loc->isErrorCodeValid(errorCode, path)){
		if (it == _errorPagesLocation.end())
			return INTERNALSERVERROR;
		path = _errorPagesLocation[errorCode];	
	}
	return OK;
}

int Router::checkAllowedMethod(std::string const &method, Location *loc){
	int perm;
	if (loc)
		perm = loc->isMethodAllowed(method);
	else
		perm = NOT_FOUND;
	if (perm == NOTFOUND)
		return METHNOTALLOWED;
	if (perm == NOT_FOUND){
		std::vector<const std::string>::const_iterator it = _allowedMethod.begin();
		for (; it != _allowedMethod.end() && *it != method;++it){}
		if (it == _allowedMethod.end())
			return METHNOTALLOWED;
	}
	return FOUND;
}

void Router::routerMain(Request *request, std::string &fullResponse, int &errorCode){
	Location *location = NULL;
	errorCode = request->getErrorCode();
	std::cout << "error code from request: " << errorCode << std::endl;
	if (request->isValid() == RESPOND)
		errorCode = getFile(request, location);
	std::cout << "error code after getfile: " << errorCode << std::endl;
	Response response(request, this, location, errorCode);
	fullResponse = response.getFullResponse();
}

int Router::getFile(Request *request, Location *&loc) {
	std::string uriCopy = request->getFilePath();
	parseUri(uriCopy);
	std::map<std::string, Location*>::const_iterator it = _locations.find(uriCopy);
	if (it != _locations.end())
		loc = _locations[uriCopy];
	int methodCode = checkAllowedMethod(request->getMethod(), loc);
	if (methodCode == METHNOTALLOWED)
		return METHNOTALLOWED;
	if (loc->getRoot(uriCopy) == NOT_FOUND)
		uriCopy = _root;
	uriCopy += request->getFilePath();
	for (int i = 0; i < 2;i++){
		int code = checkIfFileIsValid(uriCopy);
		if (code == INTERNALSERVERROR)
			return code;
		if (code == IS_DIR){
			if (uriCopy.back() != '/')
				uriCopy += "/";
			uriCopy += _index;
			request->setAddedIndex(true);
			request->setFilePath(uriCopy);
			continue;
		}
		else if (code == IS_FILE)
			break;
		else if (code == NOTFOUND)
			return code;
	}
	request->setFilePath(uriCopy);
	return (OK);
}
