#include "../include/Request.hpp"
#include "../include/color.h"
#include "../include/Server.hpp"
#include <arpa/inet.h>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <sys/signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <variant>

Request::Request(socketInfo *client, Server *server) : _client(client), _server(server), _serverName(server->getName()), _clientAddr(inet_ntoa(client->client_address.sin_addr)), _errorCode(OK), _bodyLen(0), _headerDone(false), _bodyStarted(false), _bodyEnded(false) {
	std::cout << "Created a request!" << std::endl;
	_raw.clear();
}

Request::Request(const Request &inst) {
	std::cout << "Copy Request constructor" << std::endl;
	*this = inst;
}

Request::~Request() {
	std::cout << "Request destructor" << std::endl;
	if (_fileStream.is_open())
		_fileStream.close();
}

Request& Request::operator=(const Request &rhs) {
	std::cout << "Request operator = overload" << std::endl;
	if (this != &rhs) {

	}
	return *this;
}

bool Request::_search(std::string const &searching, char endChar, std::string &result) {
	size_t start = _line.find(searching);
	if (start != std::string::npos) {
		start += searching.size();
		size_t end = _line.find(endChar, start);
		if (end != std::string::npos) {
			result = _line.substr(start, end - start);
			return true;
		}
	}
	return false;
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

void Request::headerParser() {
	std::stringstream ss(_raw);
	while(std::getline(ss, _line)) {
		if (_line == "\r") {
			std::string tmpRaw;
			while (std::getline(ss, _line)) {
				std::cout << "found the end" << std::endl; 
				tmpRaw += _line + "\n";
			}
			_raw = tmpRaw;
			_headerDone = true;
			return;
		}
		if (_method.empty() && _uri.empty()) {
			std::stringstream firstLineStream(_line);
			std::getline(firstLineStream, _method, ' ');
			std::getline(firstLineStream, _uri, ' ');
			_uriParser();
		}
		if (_host.empty() && _port.empty())
			_setHostPort();
		if (_type.empty())
			_search("Content-Type: ", ';', _type);
		if (!_bodyLenFound) {
			std::string temp;
			if (_search("Content-Length: ", '\r', temp)) {
				_bodyLen = std::stoul(temp);
				_bodyLenFound = true;
			}
		}
		if (_boundary.empty()) {
			_search("boundary=", '\r', _boundary);
		}
	}
}

void Request::addData(std::string const &data, size_t const &nbytes) {
	_raw.append(data, sizeof(nbytes));
	if (!_headerDone) {
		std::cout << "making the header of the request" << std::endl;
		headerParser();
	}
	if (_headerDone && _method == "POST" && !_raw.empty()) {
		std::cout << "header is done!" << std::endl;
		addBody();
	}
}

void Request::_setHostPort() {
		size_t hostStart = _line.find("Host: ");
		if (hostStart != std::string::npos) {
			hostStart += 6;
			size_t hostEnd = _line.find(":", hostStart);
			if (hostEnd != std::string::npos) {
				_host = _line.substr(hostStart, hostEnd - hostStart);
				if (_port.empty()) {
					size_t portEnd = _line.find('\n', hostEnd);
					if (portEnd != std::string::npos) {
						_port = _line.substr(hostEnd + 1, portEnd - hostEnd);
					}
				}
			}
		}
}

int Request::generateTempFile() {
	std::string tmpFileName = "tmp";
	_tempFilePath = "./uploads/";
	for (size_t fileIndex = 0; fileIndex < std::numeric_limits<size_t>::max(); fileIndex++) {
		int error = access((_tempFilePath + tmpFileName + std::to_string(fileIndex)).c_str(), F_OK);
		if (error != 0) {
			_tempFilePath += tmpFileName + std::to_string(fileIndex);
			_fileStream.open(_tempFilePath);
			if (!_fileStream.is_open()) {
				_tempFilePath.clear();
				return INTERNALSERVERROR;
			}
			return OK;
		}
	}
	_tempFilePath.clear();
	return INTERNALSERVERROR;
}

int Request::_findMostOfEndBoundary(size_t & pos, std::string const & string, size_t & resultLength) {
	std::string search;
	if ((pos = _raw.find(string)) != std::string::npos) {
		resultLength = string.length();
		return (WHOLE);
	}
	if (string.length() > _raw.length()) {
		search = string.substr(0, _raw.length());
	} else {
		search = string;
	}
	size_t end = search.length();
	size_t length = _raw.length() - search.length();
	for (size_t i = 0; i < end; i++) {
			std::string endString = _raw.substr(length + i);
			if (endString == search) {
				pos = length + i;
				resultLength = search.length();
				return PART;
			}
			search = search.substr(0, end - i);
	}
	return NONE;
}

void Request::addBody() {
	if (_errorCode == OK && _tempFilePath.empty()) {
		if (generateTempFile() == INTERNALSERVERROR) {
			return;
		}
	}
	if (_errorCode == OK && !_tempFilePath.empty() && _fileStream.is_open()) {
		std::string data;
		std::string firstBoundary = "--" + _boundary + "\r\n";
		std::string lastBoundary = "--" + _boundary + "--\r\n";
		size_t pos = 0;
		size_t length = 0;
		if (!_bodyStarted && _raw.find(firstBoundary) != std::string::npos)
			_bodyStarted = true;
		int code = _findMostOfEndBoundary(pos, lastBoundary, length);
		if (code == WHOLE) {
			data = _raw.substr(0, pos + length);
			_raw = _raw.substr(pos + length);
			_bodyEnded = true;
		} else if (code == PART) {
			data = _raw.substr(0, pos);
			_raw = _raw.substr(pos);
		} else {
			data = _raw;
			_raw.clear();
		}
		_fileStream << data;
		_fileStream.flush();
		if (!_raw.empty() && code == WHOLE) {
			_client->requests.push_back(new Request(_client, _server));
			_client->requests.back()->addData(_raw, _raw.size());
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
	return (!_fileStream.is_open() && _bodyEnded && _bodyStarted); 
}

std::string const &Request::getFileContent() const {
	return _fileContent;
}

std::string const &Request::getFileName() const {
	return _fileName;
}

int const &Request::getErrorCode() const {
	return _errorCode;
}

bool Request::isHeaderDone() const {
	return _headerDone;
}

bool Request::isNeedAnswer() {
	bool temp;
	if (_needAnswer == true)
		temp = true;
	else
		temp = false;
	_needAnswer = false;
	return temp;
}


int Request::isValid() const {
	if (_headerDone) {
		if (_method == "POST" && isBodyValid())
			return RESPOND;
		if (_method == "POST" && !_bodyStarted)
			return NEEDANSWER;
		else if (_bodyStarted && !_bodyEnded)
			return WAIT;
		return RESPOND;
	}
	return WAIT;
}