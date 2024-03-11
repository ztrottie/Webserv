#include "../include/Cgi.hpp"
#include <iostream>

Cgi::Cgi() {
	std::cout << "Default Cgi constructor " << std::endl;
}

Cgi::Cgi(const Cgi &inst) {
	std::cout << "Copy Cgi constructor " << std::endl;
}

Cgi::~Cgi() {
	std::cout << "Cgi destructor" << std::endl;
}

Cgi& Cgi::operator=(const Cgi &rhs) {
	std::cout << "Cgi operator = overide" << std::endl;
	if (this != &rhs) {
	}
	return *this;
}

void Cgi::env(Request *request, Router *router){
	std::string gatewayInterface = "GATEWAY_INTERFACE=CGI/1.1";
	std::string requestMethod = "REQUEST_METHOD=";
	requestMethod += request->getMethod();
	std::string queryString = "QUERY_STRING=";
	queryString += request->getStringQuerry();
	std::string contentType = "CONTENT_TYPE=";
	contentType += request->getType();
	std::string contentLength = "CONTENT_LENGTH=";
	contentLength += request->getBodyLen();
	std::string clientAddr = "REMOTE_ADDR=";
	clientAddr += request->getClientAddress();
	std::string requestURI = "REQUEST_URI=";
	requestURI += request->getUri();
	std::string serverProtocol = "SERVER_PROTOCOL=HTTP/1.1";
	std::string serverSoftware = "SERVER_SOFTWARE=webserv/1.0";
	std::string serverName = "SERVER_NAME=";
	serverName += request->getServerName();
	std::string serverPort = "SERVER_PORT=";
	serverPort += request->getPort();

	const char *env[] = {gatewayInterface.c_str(), requestMethod.c_str(), queryString.c_str(), contentType.c_str(), contentLength.c_str(), clientAddr.c_str(), requestURI.c_str(), serverProtocol.c_str(), serverSoftware.c_str(), serverName.c_str(), serverPort.c_str()};
	for (int i = 0; i < 12;i++)
		_env[i] = env[i];
}

char** Cgi::execute(Request *request){
	const char *argv[] = {"/usr/bin/php", request->getFilePath().c_str(), NULL};

}
