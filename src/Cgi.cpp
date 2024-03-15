#include "../include/Cgi.hpp"

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

void Cgi::execute(Request *request, std::string const &bodyPath){
	const char *argv[] = {"/usr/bin/php", request->getFilePath().c_str(), NULL};
	if (!request->getTempFilePath().empty())
		_inputFd = open(request->getTempFilePath().c_str(), O_RDWR);
	std::cout << request->getTempFilePath() << std::endl;
	_outputFd = open(bodyPath.c_str(), O_RDWR);
	int pid = fork();
	int status;
	env(request);
	if (pid == 0){
		if (!request->getTempFilePath().empty() && _inputFd == -1){
			std::cout << "Error\n cannot open infile!" << std::endl;
			return ;
		}
		if (_outputFd == -1){
			std::cout << "Error\n cannot open outfile!" << std::endl;
			return ;
		}
		if (pid == -1){
			std::cout << "Error while forking ⬆️➡️⬇️⬇️⬇️ on hiroshima" << std::endl;
			return ;
		}
		dup2(_inputFd, STDIN_FILENO);
		if (!request->getTempFilePath().empty())
			close(_inputFd);
		if (dup2(_outputFd, STDOUT_FILENO) == -1){
			std::cout << "Cannot dup2 fd for some reason!" << std::endl;
			return ;
		}
		close(_outputFd);
		status = execve(argv[0], const_cast<char* const *>(argv), const_cast<char * const *>(_env));
		exit(0);
	} else {
		waitpid(pid, &status, 0);
	}
}
