#include "../include/Webserv.hpp"
#include <iostream>
#include <sys/event.h>
#include <utility>

Webserv::Webserv() : _nbSockets(0) {
	std::cout << "Default Webserv constructor " << std::endl;
	_kq = kqueue();
	int socket = -2;
	Server defaultServer(socket);
	_nbSockets++;
	socket_in socketInfo = {socket, &defaultServer, SERVER};
	_clientMap.insert(std::make_pair(socket, &socketInfo));
	EV_SET(&_event, socket, EVFILT_READ | EVFILT_WRITE, EV_ADD, 0, 0, NULL);
	if (kevent(_kq, &_event, 1, NULL, 0, NULL) < 0) {
		std::cout << "fuck" << std::endl;
	}
	struct kevent event[_nbSockets];
	while (1) {
		int num_event = kevent(_kq, NULL, 0, event, 1, NULL);
		if (num_event > 0) {
			std::cout << "connection request" << std::endl;
			for (int i = 0; i < _nbSockets; i++) {
				struct socket_in *tmpServerInfo = _clientMap[event[i].ident];
				if (tmpServerInfo->type == SERVER) {
					int tmpSocket = tmpServerInfo->servInst->acceptConnection();
					
				}
			}
		}
	}
}

Webserv::Webserv(const Webserv &inst) {
	std::cout << "Copy Webserv constructor " << std::endl;
	*this = inst;
}

Webserv::~Webserv() {
	std::cout << "Webserv destructor" << std::endl;
}

Webserv& Webserv::operator=(const Webserv &rhs) {
	std::cout << "Webserv operator = overload" << std::endl;
	if (this != &rhs) {

	}
	return *this;
}
