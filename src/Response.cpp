#include "../include/Response.hpp"
#include "../include/struct.hpp"
#include "../include/Cgi.hpp"
#include <iostream>
#include <ostream>
#include <sstream>
#include <dirent.h>
#include "../include/color.h"

Response::Response(Request *request) {
	int errorPageCode;
	int errorCode = request->getErrorCode();
	if (errorCode >= 300) {
		std::string path;
		if (errorCode != INTERNALSERVERROR)
			errorPageCode = request->getRouter()->getErrorPage(path, errorCode, request->getLocation());
		else
			errorPageCode = INTERNALSERVERROR;
		if (errorPageCode == INTERNALSERVERROR || openPath(path) >= 300) {
			internalServerError(errorPageCode);
			headerGenerator(errorPageCode, request);
			_fullResponse = _header + _body;
			return;
		}
		contentTypeGenerator(path);
	} else if (request->isCgi() && request->isValid() == RESPOND) {
		handleCgi(request, errorCode);
	} else if (request->getMethod() == "GET" && request->isValid() == RESPOND) {
		handleGet(request, errorCode);
	} else if (request->getMethod() == "DELETE" && request->isValid() == RESPOND) {
		handleDelete(request, errorCode);
	} else if (request->getMethod() == "POST" && request->isValid() == RESPOND) {
		handlePost(request, errorCode);
	}
	headerGenerator(errorCode, request);
	_fullResponse = _header + _body;
}

Response::Response() {
}

Response::~Response() {
	std::cout << "Response destructor" << std::endl;
}

const std::string & Response::getFullResponse() const {
	return _fullResponse;
}


int Response::openPath(std::string const &path) {
	if (access(path.c_str(), F_OK) != 0)
		return (NOTFOUND);
	if (access(path.c_str(), R_OK) != 0)
		return (INTERNALSERVERROR);

	std::ifstream file(path, std::ios::binary);
	if (!file.is_open()) {
		return INTERNALSERVERROR;
	}
	std::ostringstream oss;
    oss << file.rdbuf();
    _body = oss.str();
	file.close();

	return (OK);
}

void Response::setBody(std::string const & body) {
	_body = body;
}

void Response::internalServerError(int & errorCode) {
	_body = "Internal Server Error";
	_contentType = "text/plain";
	errorCode = INTERNALSERVERROR;
}

void Response::contentTypeGenerator(std::string const &path) {
	std::map<std::string, std::string> contentTypeMap;

	contentTypeMap[".html"] = "text/html";
	contentTypeMap[".css"] = "text/css";
	contentTypeMap[".ico"] = "image/ico";
	contentTypeMap[".png"] = "image/png";
	contentTypeMap[".php"] = "application/php";
	int pos = path.rfind('.');
	std::string fileExtension = path.substr(pos, path.size());
	std::map<std::string, std::string>::const_iterator it = contentTypeMap.find(fileExtension);
	_contentType = ((it == contentTypeMap.end()) ? "text/plain" : contentTypeMap[fileExtension]);
}

void Response::handleGet(Request *request, int &errorCode) {
	std::string path;
	errorCode = request->getRouter()->getFile(request, path);
	if (errorCode == OK)
		errorCode = openPath(path);
	if (errorCode == INTERNALSERVERROR) {
		internalServerError(errorCode);
		return;
	} else if (errorCode >= 300 && request->getLocation()->getAutoIndex() == false) {
		std::string errorPath;
		int errorPageCode = request->getRouter()->getErrorPage(errorPath, errorCode, request->getLocation());
		if (errorPageCode == INTERNALSERVERROR || openPath(errorPath) >= 300)
			internalServerError(errorCode);
		contentTypeGenerator(errorPath);
		return;
	} else if (errorCode != 200 && request->getLocation()->getAutoIndex()) {
		directoryListing(request, errorCode);
		if (errorCode == INTERNALSERVERROR)
			return;
		if (errorCode >= 300){
			std::string errorPath;
			int errorPageCode = request->getRouter()->getErrorPage(errorPath, errorCode, request->getLocation());
			if (errorPageCode == INTERNALSERVERROR || openPath(errorPath) >= 300)
				internalServerError(errorCode);
			contentTypeGenerator(errorPath);
			return;
		}
		return;
	}
	contentTypeGenerator(path);
}

void Response::handleDelete(Request *request, int &errorCode) {
	std::string path = request->getRouter()->getRoot(request->getLocation());
	std::string filePath = request->getFilePath();
	if (path.back() == '/' && filePath.front() == '/') {
		path += filePath.substr(1);
	}
	if (access(path.c_str(), F_OK) != 0) {
		errorCode = NOTFOUND;
		std::string errorPath;
		int errorPageCode = request->getRouter()->getErrorPage(errorPath, errorCode, request->getLocation());
		if (errorPageCode == INTERNALSERVERROR || openPath(errorPath) >= 300) {
			internalServerError(errorCode);
			return;
		}
		contentTypeGenerator(errorPath);
		return;
	} else {
		if (std::remove(path.c_str()) != 0)
			internalServerError(errorCode);
		std::string newPath = request->getFilePath();
		size_t end = newPath.rfind("/");
		if (end != std::string::npos) {
			newPath = newPath.substr(0, end);
			request->setFilePath(newPath);
		}
		if (request->getLocation()->getAutoIndex()) {
			directoryListing(request, errorCode);
		} else {
			_body = "yessir";
			_contentType = "text/plain";
			errorCode = OK;
		}
	}
}

std::string Response::_headGenerator() const {
	std::string head =
	"<!DOCTYPE html>\r\n"
	"<html>\r\n"
	"<head>\r\n"
	"\t<title>Directory Listing</title>\r\n"
	"</head>\r\n"
	"<body>\r\n"
	"\t<h1>Directory Listing</h1>\r\n"
	"\t<ul>\r\n";
	return head;
}

std::string Response::_footerGenerator() const {
	std::string foot =
	"\t</ul>\r\n"
	"</body>\r\n"
	"</html>";
	return foot;
}

std::string Response::_linkGenerator(std::string const &name) const {
	std::string link;
	link += "<li><p>" + name + "</p></li>";
	return link;
}

void Response::directoryListing(Request *request, int &errorCode) {
	std::string path = request->getRouter()->getRoot(request->getLocation());
	std::string filePath = request->getFilePath();
	if (path.back() == '/' && filePath.front() == '/') {
		path += filePath.substr(1);
	}
	DIR *dir;
	struct dirent* entry;
	if (access(path.c_str(), F_OK) != 0) {
		errorCode = NOTFOUND;
		return;
	}
	dir = opendir(path.c_str());
	if (dir == NULL) {
		internalServerError(errorCode);
		return;
	}
	_body = _headGenerator();
	int i = 0;
	while ((entry = readdir(dir)) != NULL) {
		if (i >= 2)
			_body += _linkGenerator(entry->d_name);
		i++;
	}
	closedir(dir);
	_body += _footerGenerator();
	errorCode = OK;
	_contentType = "text/html";
}

void	Response::handlePost(Request *request, int &errorCode) {
	if (request->getLocation()->getUploadEnable()) {
		handleUploadedFile(request, errorCode);
	} else {
		errorCode = METHNOTALLOWED;
		std::string errorPath;
		int errorPageCode = request->getRouter()->getErrorPage(errorPath, errorCode, request->getLocation());
		if (errorPageCode == INTERNALSERVERROR || openPath(errorPath) >= 300) {
			internalServerError(errorCode);
			return;
		}
		contentTypeGenerator(errorPath);
		return;
	}
}

void	Response::handleUploadedFile(Request *request, int &errorCode) {
	int tempFileFd = open(request->getTempFilePath().c_str(), O_RDONLY);
	std::string filePath = request->getLocation()->getUploadStore();
	if (filePath.back() != '/')
		filePath += "/";
	filePath += request->getFileName();
	int fileFd = open(filePath.c_str(), O_CREAT | O_WRONLY, 0644); 
	if (tempFileFd < 0 || fileFd < 0) {
		if (tempFileFd > 0)
			close(tempFileFd);
		if (fileFd > 0)
			close(fileFd);
		internalServerError(errorCode);
		return;
	}
	ssize_t nbytes = 1024;
	char buffer[1024];
	while (1) {
		nbytes = read(tempFileFd, buffer, sizeof(buffer));
		if (nbytes <= 0)
			break;
		write(fileFd, buffer, nbytes);
	}
	close(fileFd);
	std::remove(request->getTempFilePath().c_str());
	if (nbytes < 0) {
		internalServerError(errorCode);
		return;
	}
	request->getRouter()->getErrorPage(filePath, errorCode, request->getLocation());
	if (openPath(filePath) >= 300) {
		internalServerError(errorCode);
		return;
	}
	contentTypeGenerator(filePath);
}

void Response::headerGenerator(int &errorCode, Request *request) {
	std::string location;
	std::string contentType;
	std::string	contentLength;
	std::string connection;
	if (request->getLocation()->getRedirection() && request->isValid() == RESPOND) {
		errorCode = request->getLocation()->getRedirectionCode();
		location += "Location: " + request->getLocation()->getRedirectionLocation() + "\r\n";
		_body.clear();
	} else {
		contentType = "Content-Type: " + _contentType += "\r\n";
		contentLength = "Content-Length: " + std::to_string(_body.size()) + "\r\n";
	}
	std::string codeMessageString = "HTTP/1.1 ";
	codeMessage(errorCode, codeMessageString);
	codeMessageString += "\r\n";
	std::string serverName = "Server: " + request->getServerName() + "\r\n";
	_header += codeMessageString + serverName + location +  contentType + contentLength + "\r\n";
}

void Response::codeMessage(int code, std::string &message) {
	switch (code) {
		case (OK):
			message += "200 OK";
			break;
		case (CREATED):
			message += "201 CREATED";
			break;
		case (ACCEPTED):
			message += "202 ACCEPTED";
			break;
		case (NOCONTENT):
			message += "204 No Content";
			break;
		case (RESETCONTENT):
			message += "205 Reset Content";
			break;
		case (PARTIALCONTENT):
			message += "206 Partial Content";
			break;
		case (MULTIPLECHOICE):
			message += "300 Multiple Choices";
			break;
		case (MOVEDPERM):
			message += "301 Moved Permanently";
			break;
		case (FOUND):
			message += "302 Found";
			break;
		case (SEEOTHER):
			message += "303 See Other";
			break;
		case (NOTMODIFIED):
			message += "304 Not Modified";
			break;
		case (USEPROXY):
			message += "305 Use Proxy";
			break;
		case (TEMPRED):
			message += "307 Temporary Redirect";
			break;
		case (BADREDQUEST):
			message += "400 Bad Request";
			break;
		case (UNAUTHORIZED):
			message += "401 Unauthorized";
			break;
		case (FORBIDDEN):
			message += "403 Forbidden";
			break;
		case (NOTFOUND):
			message += "404 Not Found";
			break;
		case (METHNOTALLOWED):
			message += "405 Method Not Allowed";
			break;
		case (NOTACCEPTABLE):
			message += "406 Not Acceptable";
			break;
		case (TOOLARGE):
			message += "413 Request Entity Too Large";
			break;
		case (INTERNALSERVERROR):
			message += "500 Internal Server Error";
			break;
	}
}

void Response::handleCgi(Request *request, int &errorCode) {
	std::string bodyResponse;
	int			fd = -1;
	std::string path = request->getRouter()->getRoot(request->getLocation());
	std::string filePath = request->getFilePath();
	if (path.back() == '/' && filePath.front() == '/') {
		path += filePath.substr(1);
	}
	request->setFilePath(path);
	bool isValid = (access(path.c_str(), F_OK) == 0);
	errorCode = request->generateTempFile(bodyResponse, fd);
	if (errorCode >= 300 || bodyResponse.empty() || fd < 0 || !isValid) {
		if (errorCode == OK && !isValid)
			errorCode = FORBIDDEN;
		else if (errorCode == OK && isValid)
			errorCode = NOTFOUND;
		if (fd > 0) {
			close(fd);
			std::remove(bodyResponse.c_str());
		}
		std::string errorPath;
		int errorPageCode = request->getRouter()->getErrorPage(errorPath, errorCode, request->getLocation());
		if (errorPageCode == INTERNALSERVERROR || openPath(errorPath) >= 300) {
			internalServerError(errorCode);
			return;
		}
		contentTypeGenerator(errorPath);
		return;
	}
	close(fd);
	Cgi cgi;
	cgi.execute(request, bodyResponse);
	if (openPath(bodyResponse) >= 300) {
		std::remove(request->getTempFilePath().c_str());
		std::remove(bodyResponse.c_str());
		internalServerError(errorCode);
		return;
	}
	std::remove(request->getTempFilePath().c_str());
	std::remove(bodyResponse.c_str());
}