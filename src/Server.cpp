#include "../include/Server.hpp"
#include "../include/Response.hpp"

Server::Server(uint16_t port, const char *host, std::string name, Router *router, unsigned int const &clientBodySize, socketInfo *server) : _port(port), _host(host), _name(name), _clientBodySize(clientBodySize) {
	std::cout << YELLOW << timestamp() << " Initializing a Server named " << _name << " on " << _host << ":" << _port << RESET << std::endl;
	server->socket = socket(AF_INET, SOCK_STREAM, 0);
	int reuse = 1;
	if (setsockopt(server->socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
		throw std::invalid_argument("Error setting setsockopt");
	}
	if (server->socket == -1)
		throw std::invalid_argument("Socket creation Failed");
	fcntl(server->socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(_host);
	serverAddr.sin_port = htons(_port);
	if (bind(server->socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
		throw std::invalid_argument("bind creation Failed");
	listen(server->socket, 5);
	server->type = SERVER;
	_serverRouter = router;
	_listSocket = server->socket;
	std::cout << GREEN << timestamp() << " " << _name <<  " is listening on port " << _port << RESET << std::endl;
}

Server::Server(const Server &inst) {
	std::cout << "Copy Server constructor" << std::endl;
	*this = inst;
}

Server::~Server() {
	std::cout << "Server destructor" << std::endl;
	delete _serverRouter;
}

Server& Server::operator=(const Server &rhs) {
	std::cout << "Server operator = overload" << std::endl;
	if (this != &rhs) {

	}
	return *this;
}

int Server::acceptConnection(socketInfo *client) {
	std::cout << YELLOW << timestamp() << " incomming connection trying to connect to " << _name << RESET << std::endl;
	struct sockaddr_in clientAddress = {};
	socklen_t clientAdressLen = sizeof(client->client_address);
	client->type = CLIENT;
	client->socket = accept(_listSocket, reinterpret_cast<struct sockaddr*>(&clientAddress), &clientAdressLen);
	if (client->socket < 0 || fcntl(client->socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
		std::cout << RED << timestamp() << " Server: "  << _name << " had a problem with connection incomming from: " << inet_ntoa(clientAddress.sin_addr) << " closing connection!" << std::endl;
		return CLOSE;
	}
	client->client_address = clientAddress;
	client->serverInst = this;
	client->hasRequest = false;
	std::cout << GREEN << timestamp() << " incomming connnection from " << inet_ntoa(client->client_address.sin_addr) << " accepted and setted to the socket: " << client->socket << "!" << RESET << std::endl;
	return (KEEP);
}

void Server::codeMessage(int code, std::string &message) {
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
	}
}

int Server::headerGenerator(int code, std::string &header, std::string const &body, std::string const &contentTypeRaw) {
	std::string codeMessageString = "HTTP/1.1 ";
	std::string serverName = "Server: " + _name + "\r\n";
	std::string contentType = "Content-Type: " + contentTypeRaw;
	std::string	contentLength = "Content-Length: ";
	codeMessage(code, codeMessageString);
	contentLength.append(std::to_string(body.size()));
	codeMessageString += "\r\n";
	contentType += "\r\n";
	contentLength += "\r\n";
	header += codeMessageString + serverName + contentType + contentLength + "\r\n";
	return (0);
}

int Server::recieveRequest(socketInfo *client) {
	char buffer[1025];
	ssize_t totalNbytes = 0;
	ssize_t	nbytes = 1024;
	std::string data;
	while (nbytes == 1024) {
		std::memset(buffer, 0, sizeof(buffer));
		nbytes = recv(client->socket, buffer, 1024, 0);
		if (nbytes == -1)
			break;
		data += buffer;
		totalNbytes += nbytes;
	}
	std::cout << timestamp() << " client sokcet: " << client->socket << " sent " << totalNbytes << "bytes" << std::endl;
	if (totalNbytes == 0) {
		std::cout << timestamp() << RED << " client closed the connection!" << RESET << std::endl;
		return (CLOSE);
    } else if (nbytes == -1) {
		std::cout << timestamp() << RED << " problem while recieving data closing connection" << RESET << std::endl;
		return (CLOSE);
	}
	if (!client->hasRequest) {
		client->request = new Request(data, client);
		client->hasRequest = true;
	} else {
		client->request->setBody(data);
	}
	return (KEEP);
}

// void Server::handlePostMethod(socketInfo *client) {
// 	std::string response;
// 	std::string result;
// 	std::string path;
// 	int errorCode = _serverRouter->getFile(client->request, path);
// 	std::cout << GREEN << path << RESET << std::endl;
// 	if (errorCode == INTERNALSERVERROR) {
// 		internalServerError(response);
// 	}
// 	if (errorCode == OK) {
// 		const char *argv[] = {"/usr/bin/php", path.c_str(), NULL};
// 		std::string gatewayInterface = "GATEWAY_INTERFACE=CGI/1.1";
// 		std::string requestMethod = "REQUEST_METHOD=";
// 		requestMethod += client->request->getMethod();
// 		std::string queryString = "QUERY_STRING=";
// 		queryString += client->request->getStringQuerry();
// 		std::string contentType = "CONTENT_TYPE=";
// 		contentType += client->request->getType();
// 		std::string contentLength = "CONTENT_LENGTH=";
// 		contentLength += client->request->getBodyLen();
// 		std::string clientAddr = "REMOTE_ADDR=";
// 		clientAddr += inet_ntoa(client->client_address.sin_addr);
// 		std::string requestURI = "REQUEST_URI=";
// 		requestURI += client->request->getUri();
// 		std::string serverProtocol = "SERVER_PROTOCOL=HTTP/1.1";
// 		std::string serverSoftware = "SERVER_SOFTWARE=webserv/1.0";
// 		std::string serverName = "SERVER_NAME=";
// 		serverName += _name;
// 		std::string serverPort = "SERVER_PORT=";
// 		serverPort += _port;
// 		const char *envp[] = {gatewayInterface.c_str(), requestMethod.c_str(), queryString.c_str(), contentType.c_str(), contentLength.c_str(), clientAddr.c_str(), requestURI.c_str(), serverProtocol.c_str(), serverSoftware.c_str(), serverName.c_str(), serverPort.c_str(), NULL};
// 		int cgiInput[2];
// 		int cgiOutput[2];
// 		pipe(cgiInput);
// 		pipe(cgiOutput);
// 		int pid = fork();
// 		int status;
// 		write(cgiInput[1], client->request->getClientBody().c_str(), client->request->getClientBody().size());
// 		if (pid == -1) {
// 		} else if (pid == 0) {
// 			dup2(cgiInput[0], STDIN_FILENO);
// 			// dup2(cgiOutput[1], STDOUT_FILENO);
// 			close(cgiInput[0]);
// 			close(cgiInput[1]);
// 			close(cgiOutput[0]);
// 			close(cgiOutput[1]);
// 			if (execve(argv[0], const_cast<char * const *>(argv), const_cast<char * const *>(envp)) == -1) {
// 				perror("execve failed");
// 				exit(EXIT_FAILURE);
// 			}
// 			exit(0);
// 		} else {
// 			char buffer[1024];
// 			size_t	nbytes;
// 			close(cgiInput[0]);
// 			close(cgiInput[1]);
// 			close((cgiOutput[1]));
// 			while ((nbytes = read(cgiOutput[0], buffer, sizeof(buffer))) > 0) {
// 				buffer[nbytes] = 0;
// 				result.append(buffer);
// 			}
// 			close(cgiOutput[0]);
// 			waitpid(pid, &status, 0);
// 		}
// 		if (headerGenerator(errorCode, path, response)) {
// 			response.append("\r\n\r\n");
// 			response.append(result);
// 		}
// 	} else {
// 		if (errorCode != INTERNALSERVERROR && headerGenerator(errorCode, path, response))
// 			contentGenerator(path, response);
// 	}
// 	size_t totalSent = 0;
// 	std::cout << response << std::endl;
// 	while (totalSent < response.size()) {
// 		int sent = send(client->socket, response.c_str(), response.size(), 0);
// 		totalSent += sent;
// 	}
// }

int Server::handleRequest(socketInfo *client) {
	std::string body;
	std::string contentType;
	int code = _serverRouter->routerMain(client->request, body, contentType);
	std::string header;
	std::string httpResponse;
	headerGenerator(code, header, body, contentType);
	httpResponse = header + body;
	unsigned long totalSent = 0;
	while (totalSent < httpResponse.size()) {
		int sent = send(client->socket, httpResponse.c_str() + totalSent, httpResponse.size() - totalSent, 0);
		totalSent += sent;
	}
	if (client->request->getMethod() == "POST" && client->request->getClientBody().empty())
		return (KEEP);
	return (CLOSE);
}

int Server::handleClient(socketInfo *client, int type) {
	if (type == EVFILT_READ) {
		std::cout << "read request" << std::endl;
		return recieveRequest(client);
	}
	else if (client->hasRequest && type == EVFILT_WRITE) {
		std::cout << "write request" << std::endl;
		return handleRequest(client);
	}
	return KEEP;
}

Router *Server::getRouter() const {
	return _serverRouter;
}
