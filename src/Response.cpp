#include "../include/Response.hpp"
#include "../include/struct.hpp"
#include "../include/color.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include <strstream>
#include <sys/unistd.h>
#include <unistd.h>
#include <cstdio>
#include <dirent.h>

Response::Response(Request *request, Router *router, Location *location, int &errorCode) {
	if (request->getMethod() == "POST" && errorCode == OK && !request->isBodyValid()) {
		std::cout << GREEN "here!!!" RESET << std::endl;
	} else if ((errorCode >= 300 && errorCode != NOTFOUND) || (errorCode == NOTFOUND && request->getFilePath().find(".") == std::string::npos && request->getMethod() != "POST")) {
		std::string path;
		std::cout << request->getMethod() << errorCode << " error encountered sending error" << std::endl;
		int errorPageCode = router->getErrorPage(path, errorCode, location);
		if (errorPageCode == INTERNALSERVERROR || openPath(path) >= 300) {
			internalServerError(errorCode);
			headerGenerator(errorCode, request);
			_fullResponse = _header + _body;
			return;
		}
		contentTypeGenerator(path);
	} else if (request->getMethod() == "GET") {
		handleGet(request, router, location, errorCode);
	} else if (request->getMethod() == "DELETE") {
		handleDelete(request, router, location, errorCode);
	} else if (request->getMethod() == "POST") {
		handlePost(request, router, location, errorCode);
	}
	headerGenerator(errorCode, request);
	_fullResponse = _header + _body;
}

Response::Response(const Response &inst) {
	std::cout << "Copy Response constructor" << std::endl;
	*this = inst;
}

Response::Response() {
}

Response::~Response() {
	std::cout << "Response destructor" << std::endl;
}

Response& Response::operator=(const Response &rhs) {
	std::cout << "Response operator = overload" << std::endl;
	if (this != &rhs) {

	}
	return *this;
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

void Response::handleGet(Request *request, Router *router, Location *location, int &errorCode) {
	errorCode = openPath(request->getFilePath());

	std::cout << location->getAutoIndex() << std::endl;
	if (errorCode == INTERNALSERVERROR) {
		internalServerError(errorCode);
		return;
	} else if (errorCode >= 300 && location->getAutoIndex() == false) {
		std::string errorPath;
		int errorPageCode = router->getErrorPage(errorPath, errorCode, location);
		if (errorPageCode == INTERNALSERVERROR || openPath(errorPath) >= 300)
			internalServerError(errorCode);
		contentTypeGenerator(errorPath);
		return;
	} else if (errorCode != 200 && location->getAutoIndex()) {
		directoryListing(request, errorCode);
		if (errorCode == INTERNALSERVERROR)
			return;
		if (errorCode >= 300){
			std::string errorPath;
			int errorPageCode = router->getErrorPage(errorPath, errorCode, location);
			if (errorPageCode == INTERNALSERVERROR || openPath(errorPath) >= 300)
				internalServerError(errorCode);
			contentTypeGenerator(errorPath);
			return;
		}
		return;
	}
	contentTypeGenerator(request->getFilePath());
}

void Response::handleDelete(Request *request, Router *router, Location *location, int &errorCode) {
	std::string path = request->getFilePath();
	if (access(path.c_str(), F_OK) != 0) {
		errorCode = NOTFOUND;
		std::string errorPath;
		int errorPageCode = router->getErrorPage(errorPath, errorCode, location);
		if (errorPageCode == INTERNALSERVERROR || openPath(errorPath) >= 300)
			internalServerError(errorCode);
		contentTypeGenerator(errorPath);
		return;
	} else {
		if (std::remove(path.c_str()) != 0)
			internalServerError(errorCode);
		if (location->getAutoIndex())
			directoryListing(request, errorCode);
		else {
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
	size_t end = request->getFilePath().rfind("/");
	if (end == std::string::npos) {
		internalServerError(errorCode);
		return;
	}
	std::string path;
	if (end != 0)
		path = request->getFilePath().substr(0 , end);
	else
		path = "/";
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

void	Response::handlePost(Request *request, Router *router, Location *location, int &errorCode) {
	(void)router;
	if (location->getUploadEnable() && request->getFilePath().find(".php") == std::string::npos) {
		handleUploadedFile(request, location, errorCode);
	}
	// else {
	// 	handleCgi(request, location, errorCode);
	// }
}

void	Response::handleUploadedFile(Request *request, Location *location, int &errorCode) {
	request->parseBody();
	std::string path = location->getUploadStore() + "/" + request->getFileName();
	std::cout << "opening file: " << path << std::endl;
	std::ofstream file(path.c_str());

	if (!file.is_open()) {
		internalServerError(errorCode);
		return;
	}
	std::cout << "writing content" << std::endl;
	file << request->getFileContent();
	file.close();
	std::cout << "openning the uploaded file" << std::endl;
	errorCode = openPath(path);
	if (errorCode >= 300) {
		std::cout << errorCode << std::endl;
		internalServerError(errorCode);
		return;
	}
	contentTypeGenerator(path);
}

void Response::headerGenerator(int &errorCode, Request *request) {
	std::string codeMessageString = "HTTP/1.1 ";
	std::string serverName = "Server: " + request->getServerName() + "\r\n";
	std::string contentType = "Content-Type: " + _contentType;
	std::string	contentLength = "Content-Length: ";
	codeMessage(errorCode, codeMessageString);
	contentLength.append(std::to_string(_body.size()));
	codeMessageString += "\r\n";
	contentType += "\r\n";
	contentLength += "\r\n";
	_header += codeMessageString + serverName + contentType + contentLength + "\r\n";
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
	}
}

// void Response::handleCgi(Request *request, Location *location, int &errorCode) {
	
// }