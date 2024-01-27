#include "../include/Webserv.hpp"
#include <atomic>
#include <iostream>
#include <sys/event.h>
#include <utility>
#include <unistd.h>

Webserv::Webserv() : _nbSockets(0) {
	// std::cout << "Default Webserv constructor " << std::endl;
	_kq = kqueue();
}

Webserv::Webserv(const Webserv &inst) {
	std::cout << "Copy Webserv constructor " << std::endl;
	*this = inst;
}

Webserv::~Webserv() {
	// std::cout << "Webserv destructor" << std::endl;
	close(_kq);
}

Webserv& Webserv::operator=(const Webserv &rhs) {
	std::cout << "Webserv operator = overload" << std::endl;
	if (this != &rhs) {

	}
	return *this;
}

void Webserv::addNewServer(uint16_t port, const char *host, std::string name) {
	serverInfo *server = new serverInfo;
	server->serverInst = new Server(port, host, name, server);
	_clientMap.insert(std::make_pair(server->socket, server));
	struct kevent changes;
	EV_SET(&changes, server->socket, EVFILT_READ, EV_ADD, 0, 0, nullptr);
	kevent(_kq, &changes, 1, nullptr, 0, nullptr);
	_nbSockets++;
}

void Webserv::acceptConnection(serverInfo *info) {
	serverInfo *client = new serverInfo;
	info->serverInst->acceptConnection(client);
	struct kevent clientChanges;
	EV_SET(&clientChanges, client->socket, EVFILT_READ, EV_ADD, 0, 0, nullptr);
	kevent(_kq, &clientChanges, 1, nullptr, 0, nullptr);
	_nbSockets++;
	_clientMap.insert(std::make_pair(client->socket, client));
}

void Webserv::loop() {
	while (1) {
		std::cout << _clientMap.size() << std::endl;
		struct kevent events[10];
		int numEvents = kevent(_kq, nullptr, 0, events, 10, nullptr);
		for (int i = 0; i < numEvents; i++) {
			serverInfo *info = _clientMap[events[i].ident];
			if (info->type == SERVER) {
				acceptConnection(info);
			} else {
				if (info->serverInst->handleClient(info) == CLOSE) {
					close(info->socket);
					_clientMap.erase(info->socket);
					delete info;
				}
			}
		}
	}
}
