#include "../include/Cgi.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <sys/_types/_ssize_t.h>
#include <sys/fcntl.h>
#include <unistd.h>

Cgi::Cgi() {
	std::cout << "Default Cgi constructor " << std::endl;
}

Cgi::Cgi(const Cgi &inst) {
	(void) inst;
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

char Cgi::hexToChar(const std::string &hex){
	char *endPtr;
	return static_cast<char>(std::strtol(hex.c_str(), &endPtr, 16));
}

void Cgi::env(Request *request){
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

	const char *env[] = {gatewayInterface.c_str(), requestMethod.c_str(), queryString.c_str(), contentType.c_str(), contentLength.c_str(), clientAddr.c_str(), requestURI.c_str(), serverProtocol.c_str(), serverSoftware.c_str(), serverName.c_str(), serverPort.c_str(), NULL};
	for (int i = 0; i < 12;i++)
		_env[i] = env[i];
}

void Cgi::execute(Request *request){
	const char *argv[] = {"/usr/bin/php", request->getFilePath().c_str(), NULL};
	std::ifstream input(request->getTempFilePath());
	if (!input.is_open()){
		std::cout << "Error\n cannot open infile!!" << std::endl;
		return ;
	}
	int pid = fork();
	int status;
	int end[2];
	if (pid == 0){
		env(request);
		if (pid == -1){
			std::cout << "Error while forking ⬆️➡️⬇️⬇️⬇️ on hiroshima" << std::endl;
			return ;
		}
		while (getline(input, _str, '&')){
		std::cout << "VAS CHIER CALISS" << std::endl;
			size_t findings = _str.find("%");
			while (findings != std::string::npos){
				_str += hexToChar(_str.substr(findings + 1, 2));
			}
			std::cout << "STRING" << _str << std::endl;
		}
		if (pipe(end) == - 1){
			std::cout << "Error\n cannot pipe!" << std::endl;
			return ;
		}
		write(end[1], _str.c_str(), request->getBodyLen());
		status = execve(argv[0], const_cast<char* const *>(argv), const_cast<char * const *>(_env));
		waitpid(pid, &status, 0);
	}
}