#include "../include/Request.hpp"
#include <arpa/inet.h>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>

Request::Request(std::string const &received, socketInfo *client) : _raw(received) {
	std::stringstream ss(received);
	std::getline(ss, _method, ' ');
	std::getline(ss, _uri, ' ');
	_clientAddr = inet_ntoa(client->client_address.sin_addr);
	_setHostPort();
	_uriParser();
	if (_method == "POST") {
		_type = _search("Content-Type: ", ';');
		_boundary = _search("boundary=", '\n');
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
	size_t fileEnd;
	if (fileStart != std::string::npos) {
		fileEnd = _uri.find('/', fileStart);
		if (fileEnd != std::string::npos) {
			_filePath = _uri.substr(0, fileEnd);
		} else {
			_filePath = _uri;
		}
	}
	if (fileEnd != std::string::npos) {
		size_t extraEnd = _uri.find('?', fileEnd);
		if (extraEnd != std::string::npos) {
			_extraPath = _uri.substr(fileEnd, extraEnd - fileEnd);
		} else {
			_extraPath = _uri.substr(fileEnd, _uri.size());
		}
	}
	size_t querryStart = _uri.find('?');
	if (querryStart != std::string::npos) {
		_stringQuerry = _uri.substr(querryStart, _uri.size() - querryStart);
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
				_port = _raw.substr(hostEnd, portEnd - hostEnd);
			}
		}
	}
}

void Request::_requestBodyParser() {
	size_t bodyStart = _raw.find(_boundary);
	if (bodyStart != std::string::npos) {
		bodyStart += _boundary.size();
		size_t bodyEnd = _raw.find(_boundary + "--", bodyStart);
		if (bodyEnd != std::string::npos) {
			_clientBody = _raw.substr(bodyStart, bodyEnd - bodyStart);
		}
	}
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

std::string const &Request::getClientBody() const {
	return _clientBody;
}