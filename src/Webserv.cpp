#include "../include/Webserv.hpp"
#include <sys/event.h>

Webserv::Webserv() : _nbServer(0), _nbClients(0) {
	std::cout << GREEN << timestamp() << " Starting the webserv..." << RESET << std::endl;
	_kq = kqueue();
}

Webserv::Webserv(const Webserv &inst) {
	std::cout << "Copy Webserv constructor " << std::endl;
	*this = inst;
}

Webserv::~Webserv() {
	for (std::map<int, socketInfo*>::iterator it = _clientMap.begin(); it != _clientMap.end();) {
		struct kevent clientChanges;
		int flags;
		if (it->second->type == SERVER)
			flags = EVFILT_READ;
		else
			flags = EVFILT_READ | EVFILT_WRITE;
		EV_SET(&clientChanges, it->first, flags, EV_DELETE, 0, 0, NULL);
		kevent(_kq,	&clientChanges, 1, NULL, 0, NULL);
		close(it->first);
		if (it->second->type == SERVER)
			delete it->second->serverInst;
		delete it->second;
		std::map<int, socketInfo*>::iterator tempit = it;
		it++;
		_clientMap.erase(tempit);
	}
	std::cout << "Webserv destructor" << std::endl;
}

Webserv& Webserv::operator=(const Webserv &rhs) {
	std::cout << "Webserv operator = overload" << std::endl;
	if (this != &rhs) {

	}
	return *this;
}

void Webserv::addNewServer(uint16_t port, const char *host, std::string name, Router *router, unsigned int const &clientBodySize) {
	socketInfo *server = new socketInfo; memset(server, 0, sizeof(socketInfo));
	try {
		server->serverInst = new Server(port, host, name, router, clientBodySize, server);
	} catch (std::exception &e) {
		std::cout << RED << timestamp() << " " << e.what() << RESET << std::endl;
	}
	_clientMap.insert(std::make_pair(server->socket, server));
	struct kevent changes;
	EV_SET(&changes, server->socket, EVFILT_READ, EV_ADD, 0, 0, nullptr);
	kevent(_kq, &changes, 1, nullptr, 0, nullptr);
	_nbServer++;
}

void Webserv::acceptConnection(socketInfo *info) {
	socketInfo *client = new socketInfo; memset(client, 0, sizeof(socketInfo));
	if (info->serverInst->acceptConnection(client) == CLOSE) {
		close(client->socket);
		delete client;
		return ;
	}
	struct kevent changes[2];
	EV_SET(&changes[0], client->socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, nullptr);
	EV_SET(&changes[1], client->socket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, nullptr);
	kevent(_kq, changes, 2, nullptr, 0, nullptr);
	_nbClients++;
	_clientMap.insert(std::make_pair(client->socket, client));
}

volatile bool loopFlag = true;

void signalhandler(int signal) {
	if (signal == SIGQUIT)
		loopFlag = false;
}

void Webserv::loop() {
	std::signal(SIGQUIT, signalhandler);
	while (loopFlag) {
		struct kevent events[1];
		timespec time = {10, 0};
		int numEvents = kevent(_kq, nullptr, 0, events, 1, &time);
		for (int i = 0; i < numEvents; i++) {
			std::map<int, socketInfo*>::const_iterator it = _clientMap.find(events[i].ident);
			if (it != _clientMap.end()) {
				socketInfo *info = _clientMap[events[i].ident];
				if (info->type == SERVER) {
					acceptConnection(info);
				} else {
					if (info->serverInst->handleClient(info, events[i].filter) == CLOSE) {
						struct kevent changes;
						EV_SET(&changes, info->socket, EVFILT_READ | EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);
						kevent(_kq, &changes, 1, nullptr, 0, nullptr);
						_nbClients--;
						close(info->socket);
						_clientMap.erase(info->socket);
						delete info;
					}
				}
			}
		}
	}
}
