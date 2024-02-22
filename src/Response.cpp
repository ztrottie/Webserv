#include "../include/Response.hpp"
#include "../include/struct.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/unistd.h>
#include <unistd.h>
#include <cstdio>
#include <dirent.h>

Response::Response(Request *request, Router *router, Location *location, int &errorCode) {
	if ((errorCode >= 300 && errorCode != NOTFOUND) || (errorCode == NOTFOUND && request->getFilePath().find(".") == std::string::npos)) {
		std::string path;
		std::cout << errorCode << " error encountered sending error" << std::endl;
		int errorPageCode = router->getErrorPage(path, errorCode, location);
		if (errorPageCode == INTERNALSERVERROR || openPath(path) >= 300) {
			internalServerError(errorCode);
			return;
		}
		contentTypeGenerator(path);
	}
	else if (request->getMethod() == "GET") {
		handleGet(request, router, location, errorCode);
	}
	else if (request->getMethod() == "DELETE")
		handleDelete(request, router, location, errorCode);
	// else if (request->getMethod() == "POST")
	// 	handlePost(request, router, location, errorCode);
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

const std::string & Response::getBody() const {
	return _body;
}

const std::string & Response::getContentType() const {
	return _contentType;
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
		std::cout << errorCode << std::endl;
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
