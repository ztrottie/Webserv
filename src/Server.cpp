#include "../include/Server.hpp"
#include "../include/color.h"
#include "../include/utils.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <iosfwd>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/_endian.h>
#include <sys/_types/_socklen_t.h>
#include <sys/_types/_ssize_t.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <sstream>

Server::Server(uint16_t port, const char *host, std::string name, Router *router, socketInfo *server) : _port(port), _host(host), _name(name) {
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

int Server::contentTypeGenerator(std::string &contentType, std::string const &path) {
	std::map<std::string, std::string> contentTypeMap;

	std::cout << path << std::endl;
	contentTypeMap[".html"] = "text/html";
	contentTypeMap[".css"] = "text/css";
	contentTypeMap[".ico"] = "image/ico";
	contentTypeMap[".png"] = "image/png";
	contentTypeMap[".php"] = "application.php";
	int pos = path.rfind('.');
	std::string fileExtension = path.substr(pos, path.size());
	std::cout << fileExtension << std::endl;
	std::map<std::string, std::string>::const_iterator it = contentTypeMap.find(fileExtension);
	if (it == contentTypeMap.end()) {
		std::cout << RED << timestamp() << "Error in content Type" RESET << std::endl;
		return INTERNALSERVERROR;
	}
	contentType += contentTypeMap[fileExtension];
	return (OK);
}

void Server::contentLengthGenerator(std::string &contentLength, std::string const &path) {
	std::ifstream file(path, std::ios::binary);

	if (!file.is_open()) {
		std::cerr << RED "Cannot open file!" RESET << std::endl;
	}
	file.seekg(0, std::ios::end);
	std::streampos fileSize = file.tellg();
	file.close();
	contentLength += std::to_string(static_cast<int>(fileSize));
}

int Server::headerGenerator(int code, std::string const &path, std::string &response) {
	std::string codeMessageString = "HTTP/1.1 ";
	std::string serverName = "Server: " + _name + "\r\n";
	std::string contentType = "Content-Type: ";
	std::string	contentLength = "Content-Length: ";
	codeMessage(code, codeMessageString);
	if (contentTypeGenerator(contentType, path) == INTERNALSERVERROR) {
		internalServerError(response);
		return (INTERNALSERVERROR);
	}
	contentLengthGenerator(contentLength, path);
	codeMessageString += "\r\n";
	contentType += "\r\n";
	contentLength += "\r\n";
	response += codeMessageString + serverName + contentType + contentLength + "\r\n";
	std::cout << response << std::endl;
	return (0);
}

void Server::internalServerError(std::string &response) {
	response = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain\r\nContent-length:21\r\n\r\nInternal Server Error";
}

void Server::contentGenerator(std::string const &path, std::string &response) {
	std::ifstream file(path, std::ios::binary);

	if (!file.is_open()) {
		internalServerError(response);
		return ;
	}
	// std::string line;
	// while (std::getline(file, line)) {
	// 	response += line + "\r\n";
	// }
	std::ostringstream oss;
    oss << file.rdbuf();
    response += oss.str();
	file.close();
}

int Server::recieveRequest(socketInfo *client, std::string &data) {
	char buffer[1024];
	ssize_t totalNbytes = 0;
	ssize_t	nbytes = 1024;
	while (nbytes == 1024) {
		std::memset(buffer, 0, sizeof(buffer));
		nbytes = recv(client->socket, buffer, sizeof(buffer), 0);
		data += buffer;
		totalNbytes += nbytes;
	}
	std::cout << timestamp() << " client sokcet: " << client->socket << std::endl;
	std::cout << timestamp() << " nbytes recv: " << totalNbytes << std::endl;
	if (totalNbytes == 0) {
		std::cout << timestamp() << RED << " client closed the connection!" << RESET << std::endl;
		return (CLOSE);
    } else if (totalNbytes == -1) {
		std::cout << timestamp() << RED << " problem while recieving data closing connection" << RESET << std::endl;
		return (CLOSE);
	}
	return (KEEP);
}

int Server::handleClient(socketInfo *client, int type) {
	if (type == EVFILT_READ) {
		std::cout << "read request" << std::endl;
		return recieveRequest(client, client->data);
	}
	else if (!client->data.empty() && type == EVFILT_WRITE) {
		std::cout << "write request" << std::endl;
		std::stringstream ss(client->data);
		std::string uri;
		std::string method;
		std::getline(ss, method, ' ');
		std::getline(ss, uri, ' ');
		if (method == "POST") {
			send(client->socket, "HTTP/1.1 200 OK", 15, 0);
			return KEEP;
		} else if (method == "GET") {
			std::cout << uri << std::endl;
			std::string path;
			int code = _serverRouter->getFile(method, uri, path);
			std::string response;
			if (code == INTERNALSERVERROR)
				internalServerError(response);
			if (code != INTERNALSERVERROR && headerGenerator(code, path, response) != INTERNALSERVERROR)
				contentGenerator(path, response);
			int totalSent = 0;
			while (totalSent < response.size()) {
				int sent = send(client->socket, response.c_str() + totalSent, response.size() - totalSent, 0);
				totalSent += sent;
			}
			if (code >= 300 || code == OK)
				return (CLOSE);
			return (KEEP);
		}
		send(client->socket, "HTTP/1.1 200 OK", 15, 0);
		return (CLOSE);
	}
	return KEEP;
}

Router *Server::getRouter() const {
	return _serverRouter;
}
