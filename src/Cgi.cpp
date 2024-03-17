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
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <string>
#include <sys/fcntl.h>
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
	contentType += request->getType() + "; boundary=" + request->getBoundary();
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
	int tempInFileFd;
	std::string tempFilePath;
	request->generateTempFile(tempFilePath, tempInFileFd);
	if (tempFilePath.empty() || tempInFileFd == -1)
		return;
	std::cout << GREEN << request->getTempFilePath() << " " << request->getType() << RESET << std::endl;
	if (!request->getTempFilePath().empty() && request->getType() == "application/x-www-form-urlencoded") {
		std::ifstream input(request->getTempFilePath());
		if (!input.is_open()) {
			return;
		}
		std::string line;
		bool firstLine = true;
		while(std::getline(input, line, '&')) {
			if (!firstLine) {
				std::cout << "&";
				write(tempInFileFd, "&", 1);
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
			write(tempInFileFd, line.c_str(), line.size());
		}
		close(tempInFileFd);
	} else if (!request->getTempFilePath().empty()  && request->getType() == "multipart/form-data") {
		std::string startBoundary = "--" + request->getBoundary();
		std::string endBoundary = "\r\n--" + request->getBoundary() + "--\r\n";
		std::string bodyType =  "Content-Type: " +  request->getBodyContentType();
		std::string ContentDispo = "Content-Disposition: " + request->getBodyContentDispo() + ";";
		std::string bodyName = "name=\"" + request->getBodyName() + "\";";
		std::string fileName = "filename=\"" + request->getFileName() + "\"";
		std::string header = startBoundary + "\r\n" + ContentDispo + " " +  bodyName + " " + fileName + "\r\n" + bodyType + "\r\n\r\n";
		int tempFileFd = open(request->getTempFilePath().c_str(), O_RDONLY);
		if (tempInFileFd < 0)
			return;
		size_t nbytes = 1024;
		char buffer[1024];
		write(tempInFileFd, header.c_str(), header.size());
		while (1) {
			nbytes = read(tempFileFd, buffer, sizeof(buffer));
			if (nbytes <= 0)
				break;
			write(tempInFileFd, buffer, nbytes);
		}
		write(tempInFileFd, endBoundary.c_str(), endBoundary.size());
		close(tempInFileFd);
	}
	_outputFd = open(bodyPath.c_str(), O_RDWR);
	if (_outputFd == -1){
		std::cout << "Error\n cannot open outfile!" << std::endl;
		return ;
	}
	tempInFileFd = open(tempFilePath.c_str(), O_RDONLY);
	if (tempInFileFd == -1) {
		std::cout << "Error cannot open infile!" << std::endl;
		return;
	}
	int pid = fork();
	int status;
	env(request);
	if (pid == -1){
		std::cout << "Error while forking ⬆️➡️⬇️⬇️⬇️ on hiroshima" << std::endl;
		return ;
	} else if (pid == 0){
		if (!request->getTempFilePath().empty()) {
			dup2(tempInFileFd, STDIN_FILENO);
			close(tempInFileFd);
		}
		if (dup2(_outputFd, STDOUT_FILENO) == -1){
			std::cout << "Cannot dup2 fd for some reason!" << std::endl;
			return ;
		}
		close(_outputFd);
		status = execve(argv[0], const_cast<char* const *>(argv), const_cast<char * const *>(_env));
		exit(0);
	} else {
		if (!tempFilePath.empty()) {
			close(tempInFileFd);
			std::remove(tempFilePath.c_str());
		}
		close(_outputFd);
		waitpid(pid, &status, 0);
	}
}