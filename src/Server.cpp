#include "../include/Server.hpp"
#include "../include/Response.hpp"
#include <csignal>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <sys/signal.h>
#include <sys/socket.h>
#include <unistd.h>
#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1024
#endif


Server::Server(uint16_t port, const char *host, std::string name, Router *router, socketInfo *server) : _port(port), _host(host), _name(name), _responseSize(0) {
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
	listen(server->socket, 100);
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
	if (client->socket < 0 || fcntl(client->socket, F_SETFL | O_NONBLOCK | FD_CLOEXEC) == -1) {
		std::cout << RED << timestamp() << " Server: "  << _name << " had a problem with connection incomming from: " << inet_ntoa(clientAddress.sin_addr) << " closing connection!" << std::endl;
		return CLOSE;
	}
	client->client_address = clientAddress;
	client->serverInst = this;
	client->hasRequest = false;
	client->start = time(NULL);
	std::cout << GREEN << timestamp() << " incomming connnection from " << inet_ntoa(client->client_address.sin_addr) << " accepted and setted to the socket: " << client->socket << "!" << RESET << std::endl;
	return (KEEP);
}

int Server::recieveRequest(socketInfo *client) {
	char *buffer = new char[BUFFER_SIZE];
	char *tmp = buffer;
	ssize_t nbytes = recv(client->socket, buffer, BUFFER_SIZE, 0);
	std::memset(buffer + nbytes, 0, BUFFER_SIZE - nbytes);
	if (nbytes > 0 && (client->requests.empty() || client->requests.back()->isValid() == RESPOND)) {
		client->requests.push_back(new Request(client, this));
	}
	if (nbytes == 0) {
		delete [] tmp;
		return (KEEP);
    } else if (nbytes == -1) {
		delete [] tmp;
		return (CLOSE);
	} else if (nbytes > 0) {
		client->start = time(NULL);
	}
	client->requests.back()->addData(&buffer, nbytes);
	delete [] tmp;
	return (KEEP);
}

int Server::sendData(int const &socket) {
	signal(SIGPIPE, SIG_IGN);
	ssize_t nbytes = send(socket, _responsePtr, _responseSize, 0);
	if (nbytes == -1)
		return -1;
	_responseSize -= nbytes;
	_responsePtr += nbytes;
	signal(SIGPIPE, SIG_DFL);
	return 0;
}

int Server::handleRequest(socketInfo *client) {
	if (client->requests.back()->isValid() == WAIT)
		return (KEEP);
	else if (client->requests.back()->isValid() == RESPOND) {
		Response response(client->requests.front());
		std::string fullResponse = response.getFullResponse();
		_responseSize = fullResponse.size();
		_response = new char[_responseSize + 1];
		_responsePtr = _response;
		_response[_responseSize] = 0;
		for (size_t i = 0; i < _responseSize; i++) {
			_response[i] = fullResponse[i];
		}
	}
	if (_responseSize > 0) {
		int result = sendData(client->socket);
		if (_responseSize == 0 || result == -1) {
			_responseSize = 0;
			delete _response;
			delete client->requests.front();
			client->requests.erase(client->requests.begin());
		}
	}
	return (KEEP);
}

int Server::handleClient(socketInfo *client, int type) {
	if (type == EVFILT_READ) {
		// std::cout << "received a read request" << std::endl;
		return recieveRequest(client);
	}
	else if (!client->requests.empty() && type == EVFILT_WRITE) {
		std::cout << "received a write request" << std::endl;
		return handleRequest(client);
	}
	return KEEP;
}

Router *Server::getRouter() {
	return _serverRouter;
}

std::string const &Server::getName() const {
	return _name;
}
