#include "../include/Request.hpp"
#include "../include/color.h"
#include "../include/Server.hpp"
#include <cstddef>
#include <fstream>
#include <limits>
#include <string>
#include <sys/signal.h>
#include <unistd.h>

Request::Request(std::string const &received, socketInfo *client, Server *server) : _raw(received) {
	std::stringstream ss(received);
	std::getline(ss, _method, ' ');
	std::getline(ss, _uri, ' ');
	if (client)
		_clientAddr = inet_ntoa(client->client_address.sin_addr);
	_setHostPort();
	_uriParser();
	_serverName = server->getName();
	if (_method == "POST") {
		_type = _search("Content-Type: ", ';');
		_bodyLen = std::stoul(_search("Content-Length: ", '\r'));
		_boundary = _search("boundary=", '\r');
		_requestBodyParser();
	}
}

Request::Request(const Request &inst) {
	std::cout << "Copy Request constructor" << std::endl;
	*this = inst;
}

Request::~Request() {
	std::cout << "Request destructor" << std::endl;
}

Request& Request::operator=(const Request &rhs) {
	std::cout << "Request operator = overload" << std::endl;
	if (this != &rhs) {

	}
	return *this;
}

std::string Request::_search(std::string const &searching, char endChar) {
	size_t start = _raw.find(searching);
	if (start != std::string::npos) {
		start += searching.size();
		size_t end = _raw.find(endChar, start);
		if (end != std::string::npos)
			return _raw.substr(start, end - start);
	}
	return "";
}

void Request::_uriParser() {
	size_t fileStart = _uri.find('.');
	if (fileStart != std::string::npos) {
		size_t extraStart = _uri.find('/', fileStart);
		if (extraStart != std::string::npos) {
			_filePath = _uri.substr(0, extraStart);
			size_t queryStart = _uri.find('?', extraStart);
			if (queryStart != std::string::npos) {
				_extraPath = _uri.substr(extraStart, queryStart - extraStart);
				_stringQuerry = _uri.substr(queryStart, _uri.size() - queryStart);
			} else {
				_extraPath = _uri.substr(extraStart, _uri.size() - extraStart);
			}
		} else {
			size_t querryStart = _uri.find('?');
			if (querryStart != std::string::npos) {
				_filePath = _uri.substr(0, querryStart);
				_stringQuerry = _uri.substr(querryStart, _uri.size() - querryStart);
			} else {
				_filePath = _uri;
			}
		}
	} else {
		size_t querryStart = _uri.find('?');
		if (querryStart != std::string::npos) {
			_filePath = _uri.substr(0, querryStart);
			_stringQuerry = _uri.substr(querryStart, _uri.size() - querryStart);
		} else {
			_filePath = _uri;
		}
	}
}

void Request::_setHostPort() {
	size_t hostStart = _raw.find("Host: ");
	if (hostStart != std::string::npos) {
		hostStart += 6;
		size_t hostEnd = _raw.find(":", hostStart);
		if (hostEnd != std::string::npos) {
			_host = _raw.substr(hostStart, hostEnd - hostStart);
			size_t portEnd = _raw.find('\n', hostEnd);
			if (portEnd != std::string::npos) {
				_port = _raw.substr(hostEnd + 1, portEnd - hostEnd);
			}
		}
	}
}

int Request::generateTempFile() {
	std::string tmpFileName = ".tmp";
	_tempFilePath = "/tmp/";
	for (size_t fileIndex = 0; fileIndex < std::numeric_limits<size_t>::max(); fileIndex++) {
		if (access((_tempFilePath + tmpFileName + std::to_string(fileIndex)).c_str(), F_OK) != 0) {
			_tempFilePath += tmpFileName + std::to_string(fileIndex);
			return OK;
		}
	}
	_tempFilePath.clear();
	return INTERNALSERVERROR;
}

void Request::parseFileName(std::string &string) {
	size_t start = string.find("filename=\"");
	if (start != std::string::npos) {
		start += 10;
		size_t end = string.find("\"", start);
		if (end != std::string::npos) {
			_fileName = string.substr(start, end - start);
		}
	}
}

void Request::_requestBodyParser() {
	std::string tempBoundary("\r\n\r\n--");
	tempBoundary.append(_boundary);
	size_t bodyStart = _raw.rfind(tempBoundary);
	if (bodyStart != std::string::npos) {
		bodyStart += tempBoundary.size() + 2;
		size_t bodyEnd = _raw.find(tempBoundary.append("--"), bodyStart);
		if (bodyEnd != std::string::npos) {
			std::string fullBody = _raw.substr(bodyStart, bodyEnd - bodyStart);
			parseFileName(fullBody);
			
		}
	}
}

void Request::addBody(std::string &body) {
	
}

std::string const &Request::getMethod() const {
	return _method;
}

std::string const &Request::getRaw() const {
	return _raw;
}

std::string const &Request::getUri() const {
	return _uri;
}

std::string const &Request::getFilePath() const {
	return _filePath;
}

std::string const &Request::getExtraPath() const {
	return _extraPath;
}

std::string const &Request::getStringQuerry() const {
	return _stringQuerry;
}

std::string const &Request::getHost() const {
	return _host;
}

std::string const &Request::getPort() const {
	return _port;
}

std::string const &Request::getType() const {
	return _type;
}

std::string const &Request::getBoundary() const {
	return _boundary;
}

std::string const &Request::getClientAddress() const {
	return _clientAddr;
}

size_t const &Request::getBodyLen() const {
	return _bodyLen;
}

std::string const &Request::getServerName() const{
	return _serverName;
}

void Request::setFilePath(std::string &path){
	_filePath = path;
}

void Request::setAddedIndex(bool index){
	_addedIndex = index;
}

bool Request::getAddedIndex() const{
	return _addedIndex;
}

bool Request::isBodyValid() const {
	
}

std::string Request::parseBody(std::string &string) {
	size_t start = string.find("\r\n\r\n");
	if (start != std::string::npos) {
		start += 4;
		std::string tempBoundary = "\r\n";
		tempBoundary += "--" + _boundary + "--";
		size_t end = string.find(tempBoundary);
		if (end != std::string::npos) {
			
		}
	}
}

std::string const &Request::getFileContent() const {
	return _fileContent;
}

std::string const &Request::getFileName() const {
	return _fileName;
}
