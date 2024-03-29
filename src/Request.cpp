#include "../include/Request.hpp"
#include "../include/color.h"
#include "../include/Server.hpp"

Request::Request(socketInfo *client, Server *server) : _client(client), _server(server), _method(""), _serverName(server->getName()), _clientAddr(inet_ntoa(client->client_address.sin_addr)), _errorCode(OK), _bodyLen(0), _headerDone(false), _bodyStarted(false), _bodyEnded(false), _bodyLenWritten(0) {
	_raw.clear();
}

Request::~Request() {
}

bool Request::_search(std::string const &searching, char endChar, std::string &result) {
	size_t start = _raw.find(searching);
	if (start != std::string::npos) {
		start += searching.size();
		size_t end = _raw.find(endChar, start);
		if (end != std::string::npos) {
			result = _raw.substr(start, end - start);
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
	size_t start = _filePath.find(".");
	if (start != std::string::npos) {
		start += 1;
		_extension = _filePath.substr(start);
	}
}

void Request::_headerParser(char **buffer) {
	size_t headerEnd = _raw.find("\r\n\r\n");
	if (headerEnd != std::string::npos) {
		headerEnd += 4;
		_headerDone = true;
		std::stringstream ss(_raw);
		std::getline(ss, _method, ' ');
		std::getline(ss, _uri, ' ');
		_uriParser();
		_setHostPort();
		std::string bodyLenString;
		_errorCode = _server->getRouter()->getLocation(this, _location, _fullPath);
		if (_search("Content-Length: ", '\r', bodyLenString)) {
			_bodyLen = std::stol(bodyLenString);
			if (_bodyLen > getRouter()->getClientMaxBodySize(_location) && _errorCode == OK)
				_errorCode = TOOLARGE;
		}
		_serverName = _server->getName();
		_clientAddr = inet_ntoa(_client->client_address.sin_addr);
		_search("boundary=", '\r', _boundary);
		if (_raw.find("boundary") != std::string::npos)
			_search("Content-Type: ", ';', _type);
		else
			_search("Content-Type: ", '\r', _type);
		if (headerEnd > _rawSize) {
			size_t nbytes = headerEnd - _rawSize;
			_nbytesRead -= nbytes;
			*buffer += nbytes;
			_rawSize = 0;
		} else {
			_rawSize -= headerEnd;
		}
		_raw = _raw.substr(headerEnd);
	}
}

void Request::addData(char **buffer, size_t const &nbytes) {
	_nbytesRead = nbytes;
	_rawSize = _raw.size();
	_raw.append(*buffer, nbytes);
	if (!_headerDone) {
		_headerParser(buffer);
	}
	if (_headerDone && _method == "POST" && !_raw.empty()) {
		addBody(buffer);
	}
	if (isValid() && !_raw.empty()) {
		_client->requests.push_back(new Request(_client, _server));
		_client->requests.back()->addData(buffer, _nbytesRead);
	}
}

void Request::_setHostPort() {
	size_t hostStart = _raw.find("Host: ");
	if (hostStart != std::string::npos) {
		hostStart += 6;
		size_t hostEnd = _raw.find(":", hostStart);
		if (hostEnd != std::string::npos) {
			_host = _raw.substr(hostStart, hostEnd - hostStart);
			if (_port.empty()) {
				size_t portEnd = _raw.find('\n', hostEnd);
				if (portEnd != std::string::npos) {
					_port = _raw.substr(hostEnd + 1, portEnd - hostEnd);
				}
			}
		}
	}
}

int Request::generateTempFile(std::string &tempFilePath, int &tempFileFd) {
	std::string tmpFileName = "tmp";
	tempFilePath = "/tmp/";
	for (size_t fileIndex = 0; fileIndex < std::numeric_limits<size_t>::max(); fileIndex++) {
		int error = access((tempFilePath + tmpFileName + std::to_string(fileIndex)).c_str(), F_OK);
		if (error != 0) {
			tempFilePath += tmpFileName + std::to_string(fileIndex);
			tempFileFd = open(tempFilePath.c_str(), O_CREAT | O_RDWR, 0644);
			if (tempFileFd == -1)
				return INTERNALSERVERROR;
			return OK;
		}
	}
	tempFilePath.clear();
	return INTERNALSERVERROR;
}

void Request::parseFileName() {
	std::string search = "filename=\"";
	size_t start = _raw.find(search);
	if (start != std::string::npos) {
		start += search.length();
		size_t end = _raw.find("\"", start);
		if (end != std::string::npos) {
			_fileName = _raw.substr(start, end - start);
		}
	}
}

void Request::ParseBodyHeader(char **buffer) {
	std::string endBoundary = "\r\n--" + _boundary + "--\r\n";
	if (!_boundary.empty()) {
		size_t headerEnd = _raw.find("\r\n\r\n");
		if (headerEnd != std::string::npos) {
			headerEnd += 4;
			_bodyStarted = true;
			if (_errorCode == OK && _fileName.empty())
				_search("filename=\"", '\"', _fileName);
			if (_errorCode == OK && _bodyName.empty())
				_search("; name=\"", '\"', _bodyName);
			if (_errorCode == OK && _bodyContentDispo.empty())
				_search("Content-Disposition: ", ';', _bodyContentDispo);
			if (_errorCode == OK && _bodyContentType.empty())
				_search("Content-Type: ", '\r', _bodyContentType);
			if (headerEnd > _rawSize) {
				size_t nbytes = headerEnd - _rawSize;
				_nbytesRead -= nbytes;
				*buffer += nbytes;
			}
			_bodyLenModified = _bodyLen - (headerEnd + endBoundary.length());
			_raw.clear();
			_rawSize = 0;
		}
	} else {
		_bodyLenModified = _bodyLen;
		_bodyStarted = true;
		_rawSize = 0;
		_raw.clear();
	}
}

void Request::addBody(char **buffer) {
	std::string endBoundary = "\r\n--" + _boundary + "--\r\n";
	if (_errorCode == OK && _tempFilePath.empty()) {
		if (generateTempFile(_tempFilePath, _tempFileFd) == INTERNALSERVERROR) {
			_errorCode = INTERNALSERVERROR;
			_bodyStarted = true;
			_bodyEnded = true;
			return;
		}
	}
	if (!_bodyStarted) {
		ParseBodyHeader(buffer);
	}
	if (_bodyStarted && _bodyLenWritten != _bodyLenModified) {
		if (_bodyLenModified < _nbytesRead + _bodyLenWritten) {
			_nbytesRead = _bodyLenModified - _bodyLenWritten;
		}
		if (_errorCode == OK) {
			while (_nbytesRead > 0) {
				size_t nbytesWritten = write(_tempFileFd, *buffer, _nbytesRead);
				_nbytesRead -= nbytesWritten;
				*buffer += nbytesWritten;
				_bodyLenWritten += nbytesWritten;
			}
		} else {
			_bodyLenWritten += _nbytesRead;
			*buffer += _nbytesRead;
		}
		if (_bodyLenWritten == _bodyLenModified) {
			_raw = *buffer;
			_rawSize = _nbytesRead;
			if (_errorCode == OK && !_tempFilePath.empty())
				close(_tempFileFd);
		}
	}
	if (_bodyLenWritten == _bodyLenModified && !_bodyEnded) {
		if (!_boundary.empty()) {
			size_t end = _raw.find(endBoundary);
			if (end != std::string::npos) {
				_bodyEnded = true;
				if (end > 0) {
					if (_errorCode == OK && !_tempFilePath.empty())
						std::remove(_tempFilePath.c_str());
					_errorCode = TOOLARGE;
				}
				if (_errorCode == OK)
					_errorCode = CREATED;
				_raw = _raw.substr(end + endBoundary.length());
			}
		} else {
			_bodyEnded = true;
			_raw = *buffer;
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

Location *Request::getLocation() {
	return _location;
}

Router *Request::getRouter() {
	return _server->getRouter();
}

void Request::setFilePath(std::string &path){
	_filePath = path;
}

bool Request::isBodyValid() const {
	return (_bodyEnded && _bodyStarted); 
}

std::string const &Request::getFileName() const {
	return _fileName;
}

std::string const &Request::getFullPath() const {
	return _fullPath;
}

std::string const &Request::getTempFilePath() const {
	return _tempFilePath;
}

std::string const &Request::getExtension() const {
	return _extension;
}

int const &Request::getErrorCode() const {
	return _errorCode;
}

bool Request::isHeaderDone() const {
	return _headerDone;
}

int Request::isValid() const {
	if (_headerDone) {
		if (_method == "POST" && !isBodyValid())
			return WAIT;
		return RESPOND;
	}
	return WAIT;
}

bool Request::isCgi() const {
	if (_extension == "py" && _location->getUseCGI()) {
		return true;
	}
	return false;
}

std::string const &Request::getBodyContentType() const {
	return _bodyContentType;
}

std::string const &Request::getBodyContentDispo() const {
	return _bodyContentDispo;
}

std::string const &Request::getBodyName() const {
	return _bodyName;
}