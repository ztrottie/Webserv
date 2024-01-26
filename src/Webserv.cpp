#include "../include/Webserv.hpp"
#include <iostream>
#include <sys/event.h>
#include <utility>
#include <unistd.h>

Webserv::Webserv() : _nbSockets(0) {
	std::cout << "Default Webserv constructor " << std::endl;
	_kq = kqueue();
}

Webserv::Webserv(const Webserv &inst) {
	std::cout << "Copy Webserv constructor " << std::endl;
	*this = inst;
}

Webserv::~Webserv() {
	std::cout << "Webserv destructor" << std::endl;
	close(_kq);
}

Webserv& Webserv::operator=(const Webserv &rhs) {
	std::cout << "Webserv operator = overload" << std::endl;
	if (this != &rhs) {

	}
	return *this;
}

void Webserv::addNewServer(uint16_t port, const char *host, std::string name) {
	serverInfo server;
	server.serverInst = new Server(port, host, name, &server);
	_clientMap.insert(std::make_pair(server.socket, &server));
	struct kevent changes[1];
	EV_SET(&changes[0], server.socket, EVFILT_READ, EV_ADD, 0, 0, nullptr);
	kevent(_kq, changes, 1, nullptr, 0, nullptr);
	_nbSockets++;
}

void Webserv::loop() {
	while (1) {
		struct kevent events[10];
		int numEvents = kevent(_kq, nullptr, 0, events, 10, nullptr);
		std::cout << numEvents << std::endl;
		for (int i = 0; i < numEvents; i++) {
			serverInfo *server = _clientMap[events[i].ident];
			if (server->type == SERVER) {
				serverInfo client;
				server->serverInst->acceptConnection(&client);
				struct kevent clientChanges;
				EV_SET(&clientChanges, client.socket, EVFILT_READ | EVFILT_WRITE, EV_ADD, 0, 0, nullptr);
				kevent(_kq, &clientChanges, 1, nullptr, 0, nullptr);
				_nbSockets++;
			}
		}
	}
}
