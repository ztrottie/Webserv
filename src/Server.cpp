#include "../include/Server.hpp"
#include "../include/Response.hpp"
#include <cstring>
#include <new>

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
	listen(server->socket, 20);
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

int Server::recieveRequest(socketInfo *client) {
	char *buffer = new char[1024];
	char *tmp = buffer;
	std::memset(buffer, 0, 1024);
	ssize_t nbytes = recv(client->socket, buffer, 1024, 0);
	if (client->requests.empty() || client->requests.back()->isValid() == RESPOND) {
		client->requests.push_back(new Request(client, this));
	}
	if (nbytes == 0) {
		std::cout << timestamp() << RED << " client closed the connection!" << RESET << std::endl;
		delete [] tmp;
		return (CLOSE);
    } else if (nbytes == -1) {
		std::cout << timestamp() << RED << " problem while recieving data closing connection" << RESET << std::endl;
		delete [] tmp;
		return (CLOSE);
	}
	client->requests.back()->addData(&buffer, nbytes);
	delete [] tmp;
	return (KEEP);
}

void Server::sendAll(int const &socket, std::string const &fullResponse) {
	unsigned long totalSent = 0;
	while (totalSent < fullResponse.size()) {
		int sent = send(socket, fullResponse.c_str() + totalSent, fullResponse.size() - totalSent, 0);
		totalSent += sent;
	}
}

int Server::handleRequest(socketInfo *client) {
	std::string fullResponse;
	std::cout << RED << client->requests.back()->isValid() << RESET << std::endl;
	if (client->requests.back()->isValid() == WAIT)
		return (KEEP);
	else if (client->requests.back()->isValid() == NEEDANSWER) {
		Response response(client->requests.back());
		sendAll(client->socket, fullResponse);
		return (KEEP);
	} else {
		Response response(client->requests.front());
		sendAll(client->socket, fullResponse);
		delete client->requests.front();
		client->requests.erase(client->requests.begin());
		if (client->requests.size() > 0)
			return KEEP;
		return CLOSE;
	}
}

int Server::handleClient(socketInfo *client, int type) {
	if (type == EVFILT_READ) {
		std::cout << "read request" << std::endl;
		return recieveRequest(client);
	}
	else if (!client->requests.empty() && type == EVFILT_WRITE) {
		std::cout << "write request" << std::endl;
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
