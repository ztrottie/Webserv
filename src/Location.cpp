#include "../include/Location.hpp"

Location::Location() {
	std::cout << "Default Location constructor " << std::endl;
}

Location::Location(std::string const &name) : _name(name) {

}

Location::Location(const Location &inst) {
	(void)inst;
	std::cout << "Copy Location constructor " << std::endl;
	*this = inst;
}

Location::~Location() {
	std::cout << "Location destructor" << std::endl;
}

Location& Location::operator=(const Location &rhs) {
	(void) rhs;
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
	return OK;
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

bool Location::getAutoIndex() const{
	return _autoIndex;
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

void Location::setUploadEnable(bool enable){
	_uploadEnable = enable;
}

bool Location::getUploadEnable() const {
	return _uploadEnable;
}

void Location::setClientMaxBodySize(size_t value) {
	_clientMaxBodySize = value;
}

void Location::setAutoIndex(bool const &autoIndex) {
	_autoIndex = autoIndex;
}

const std::string	&Location::getUploadStore() const {
	return _uploadStore;
}

void Location::setUploadStore(std::string const &value) {
	_uploadStore = value;
}

const std::string	&Location::getName() const {
	return _name;
}

void Location::setRedirection(bool value){
	_redirection = value;
}

bool Location::getRedirection() const{
	return _redirection;
}

int Location::getRedirectionCode() const{
	return _redirectionCode;
}

void Location::setRedirectionCode(int code){
	_redirectionCode = code;
}

void Location::setRedirectionLocation(std::string str){
	_redirectionLocation = str;
}

std::string Location::getRedirectionLocation() const{
	return _redirectionLocation;
}

long long int Location::getClientMaxBodySize() const{
	return _clientMaxBodySize;
}

void Location::setClientMaxBodySize(long long int value){
	_clientMaxBodySize = value;
}
