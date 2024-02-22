#pragma once
#include <arpa/inet.h>
#include <string>

class Server;
class Request;

enum socketType {
	CLIENT,
	SERVER
};

enum socketStatus {
	KEEP,
	CLOSE
};

struct socketInfo {
	int					socket;
	int					type;
	int 				mode;
	Request				*request;
	bool				hasRequest;
	struct sockaddr_in	client_address;
	Server				*serverInst;
};

enum Codes {
	OK = 200,
	CREATED,
	ACCEPTED,
	NOCONTENT = 204,
	RESETCONTENT,
	PARTIALCONTENT,
	MULTIPLECHOICE = 300,
	MOVEDPERM,
	FOUND,
	SEEOTHER,
	NOTMODIFIED,
	USEPROXY,
	TEMPRED = 307,
	BADREDQUEST = 400,
	UNAUTHORIZED,
	FORBIDDEN = 403,
	NOTFOUND,
	METHNOTALLOWED,
	NOTACCEPTABLE,
	TOOLARGE = 413,
	INTERNALSERVERROR = 500,
};