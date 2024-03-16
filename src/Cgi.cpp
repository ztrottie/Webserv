#include "../include/Cgi.hpp"
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <string>
#include <unistd.h>
#include "../include/color.h"

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

char Cgi::decToHex(std::string hexaString) {
	return static_cast<char>(std::strtol(hexaString.c_str(), NULL, 16));
}

void Cgi::execute(Request *request, std::string const &bodyPath){
	const char *argv[] = {"/usr/bin/python3", request->getFilePath().c_str(), NULL};
	int fds[2];
	std::ifstream input;
	if (!request->getTempFilePath().empty()) {
		input.open(request->getTempFilePath());
		if (!input.is_open()) {
			return;
		}
		if (pipe(fds) < 0) {
			return;
		}
		std::string line;
		bool firstLine = true;
		while(std::getline(input, line, '&')) {
			if (!firstLine) {
				std::cout << "&";
				write(fds[1], "&", 1);
			} else {
				firstLine = false;
			}
			size_t start = line.find("%");
			while (start != std::string::npos) {
				std::string first = line.substr(0, start);
				char mid = decToHex(line.substr(start + 1, 2));
				std::string end = line.substr(start + 3);
				line = first + mid + end;
				start = line.find("%");
			}
			std::cout << line;
			write(fds[1], line.c_str(), line.size());
		}
		std::cout << std::endl;
	}
	_outputFd = open(bodyPath.c_str(), O_RDWR);
	if (_outputFd == -1){
		std::cout << "Error\n cannot open outfile!" << std::endl;
		return ;
	}
	int pid = fork();
	int status;
	env(request);
	if (pid == -1){
		std::cout << "Error while forking ⬆️➡️⬇️⬇️⬇️ on hiroshima" << std::endl;
		return ;
	} else if (pid == 0){
		if (!request->getTempFilePath().empty()) {
			dup2(fds[0], STDIN_FILENO);
			close(fds[0]);
			close(fds[1]);
		}
		if (dup2(_outputFd, STDOUT_FILENO) == -1){
			std::cout << "Cannot dup2 fd for some reason!" << std::endl;
			return ;
		}
		close(_outputFd);
		status = execve(argv[0], const_cast<char* const *>(argv), const_cast<char * const *>(_env));
		exit(0);
	} else {
		if (!request->getTempFilePath().empty()) {
			close(fds[0]);
			close(fds[1]);
		}
		close(_outputFd);
		waitpid(pid, &status, 0);
	}
}
