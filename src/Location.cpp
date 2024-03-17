#include "../include/Location.hpp"
#include "../include/struct.hpp"
#include <iostream>

Location::Location() : _name("default"), _clientMaxBodySizeSet(false) {
	std::cout << "Default Location constructor " << std::endl;
	_redirectionCode = -1;
	_useCGI = false;
	_clientMaxBodySize = -1;
	_uploadEnableSet = false;
	_autoIndexSetter = false;
	_useCGISetter = false;
}

Location::Location(std::string const &name) : _name(name), _clientMaxBodySizeSet(false) {
	_redirectionCode = -1;
	_useCGI = false;
	_clientMaxBodySize = -1;
	_uploadEnableSet = false;
	_autoIndexSetter = false;
	_useCGISetter = false;
}

Location::Location(const Location &inst) {
	(void)inst;
	std::cout << "Copy Location constructor " << std::endl;
	*this = inst;
}

Location::~Location() {
	// std::cout << "Location destructor" << std::endl;
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
		return INTERNALSERVERROR;
	std::vector<const std::string>::const_iterator it = _allowedMethod.begin();
	for (; it != _allowedMethod.end() && *it != method;++it){}
	if (it == _allowedMethod.end())
		return FORBIDDEN;
	return OK;
}

int Location::isErrorCodeValid(int const &errorCode, std::string &path){
	std::map<int, std::string>::const_iterator it = _errorPagesLocation.find(errorCode);
	if (it == _errorPagesLocation.end())
		return NOTFOUND;
	path = _errorPagesLocation[errorCode];
	return FOUND;
}

int Location::getRoot(std::string &root){
	if (_root.empty())
		return NOTFOUND;
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
	_uploadEnableSet = true;
}
bool	Location::getUploadEnableSetter() const{
	return _uploadEnableSet;
}

bool Location::getUploadEnable() const {
	return _uploadEnable;
}

void Location::setAutoIndex(bool const &autoIndex) {
	_autoIndex = autoIndex;
	_autoIndexSetter = true;
}

bool	Location::getAutoIndexSetter() const{
	return _autoIndexSetter;
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

void Location::setRedirectionLocation(std::string str, bool isString){
	_redirectionLocation = str;
	_redirIsString = isString;
}

std::string Location::getRedirectionLocation() const{
	return _redirectionLocation;
}

void Location::getClientMaxBodySize(size_t &clientMaxBodySize) const {
	if (_clientMaxBodySizeSet)
		clientMaxBodySize = _clientMaxBodySize;
}

void Location::setClientMaxBodySize(size_t value){
	_clientMaxBodySizeSet = true;
	_clientMaxBodySize = value;
}

std::string Location::getAllowedMethods() const{
	std::string res = "";
	for (size_t i = 0; i < _allowedMethod.size(); i++){
		res = res + " " + _allowedMethod[i];
	}
	return res;
}

std::string	Location::getIndex() const{
	return _index;
}

void	Location::setUseCGI(bool value){
	_useCGI = value;
	_useCGISetter = true;
}


bool	Location::getUseCGI() const{
	return _useCGI;
}

bool	Location::getUseCGISetter() const{
	return _useCGISetter;
}

std::string	Location::getRootParsing() const{
	return _root;
}

std::string Location::getErrorParsing(int code) const{
	std::map<int, std::string>::const_iterator it = _errorPagesLocation.find(code);
	return it->second;
}

long long	Location::getClientMaxBodySizeParsing() const{
	return _clientMaxBodySize;
}
