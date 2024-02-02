#include "../include/Webserv.hpp"
#include <csignal>
#include <cstddef>
#include <ctime>
#include <exception>
#include <iostream>
#include <sys/event.h>
#include <sys/signal.h>
#include <utility>
#include <unistd.h>
#include "../include/utils.hpp"

Webserv::Webserv() : _nbServer(0), _nbClients(0) {
	std::cout << GREEN << timestamp() << " Starting the webserv..." << RESET << std::endl;
	_kq = kqueue();
}

Webserv::Webserv(const Webserv &inst) {
	std::cout << "Copy Webserv constructor " << std::endl;
	*this = inst;
}

Webserv::~Webserv() {
	std::cout << "Webserv destructor" << std::endl;
	for (std::map<int, serverInfo*>::iterator it = _clientMap.begin(); it != _clientMap.end();) {
		struct kevent clientChanges;
		EV_SET(&clientChanges, it->first, EVFILT_READ, EV_DELETE, 0, 0, NULL);
		kevent(_kq,	&clientChanges, 1, NULL, 0, NULL);
		close(it->first);
		if (it->second->type == SERVER)
			delete it->second->serverInst;
		delete it->second;
		std::map<int, serverInfo*>::iterator tempit = it;
		it++;
		_clientMap.erase(tempit);
	}
	close(_kq);
	std::cout << "tessir" << std::endl;
}

Webserv& Webserv::operator=(const Webserv &rhs) {
	std::cout << "Webserv operator = overload" << std::endl;
	if (this != &rhs) {

	}
	return *this;
}

void Webserv::addNewServer(uint16_t port, const char *host, std::string name, Router *router) {
	serverInfo *server = new serverInfo;
	try {
		server->serverInst = new Server(port, host, name, router, server);
	} catch (std::exception &e) {
		std::cout << RED << timestamp() << " " << e.what() << RESET << std::endl;
	}
	_clientMap.insert(std::make_pair(server->socket, server));
	struct kevent changes;
	EV_SET(&changes, server->socket, EVFILT_READ, EV_ADD, 0, 0, nullptr);
	kevent(_kq, &changes, 1, nullptr, 0, nullptr);
	_nbServer++;
}

void Webserv::acceptConnection(serverInfo *info) {
	serverInfo *client = new serverInfo;
	if (info->serverInst->acceptConnection(client) == CLOSE) {
		close(client->socket);
		delete client;
		return ;
	}
	struct kevent clientChanges;
	EV_SET(&clientChanges, client->socket, EVFILT_READ, EV_ADD, 0, 0, nullptr);
	kevent(_kq, &clientChanges, 1, nullptr, 0, nullptr);
	_nbClients++;
	_clientMap.insert(std::make_pair(client->socket, client));
}

volatile bool loopFlag = true;

void signalhandler(int signal) {
	loopFlag = false;
}

void Webserv::loop() {
	std::signal(SIGQUIT, signalhandler);
	while (loopFlag) {
		std::cout << timestamp() << " Number of connection on the webserv: [" << _nbClients << "]" << std::endl;
		struct kevent events[10];
		std:timespec time = {10, 0};
		int numEvents = kevent(_kq, nullptr, 0, events, 10, &time);
		for (int i = 0; i < numEvents; i++) {
			serverInfo *info = _clientMap[events[i].ident];
			if (info->type == SERVER) {
				acceptConnection(info);
			} else {
				if (info->serverInst->handleClient(info) == CLOSE) {
					struct kevent clientChanges;
					EV_SET(&clientChanges, info->socket, EVFILT_READ, EV_DELETE, 0, 0, nullptr);
					kevent(_kq,	&clientChanges, 1, nullptr, 0, nullptr);
					_nbClients--;
					close(info->socket);
					_clientMap.erase(info->socket);
					delete info;
				}
			}
		}
	}
}
