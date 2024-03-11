#pragma once
#include <arpa/inet.h>
#include <fstream>
#include <string>
#include <vector>

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

enum requestCode {
	WAIT,
	RESPOND,
	NEEDANSWER
};

struct socketInfo {
	int						socket;
	int						type;
	int 					mode;
	std::vector<Request*>	requests;
	bool					hasRequest;
	struct sockaddr_in		client_address;
	Server					*serverInst;
};

enum Codes {
	OK = 200,
	CREATED = 201,
	ACCEPTED = 202,
	NOCONTENT = 204,
	RESETCONTENT = 205,
	PARTIALCONTENT = 206,
	MULTIPLECHOICE = 300,
	MOVEDPERM = 301,
	FOUND = 302,
	SEEOTHER = 303,
	NOTMODIFIED = 304,
	USEPROXY = 305,
	TEMPRED = 307,
	BADREDQUEST = 400,
	UNAUTHORIZED = 401,
	FORBIDDEN = 403,
	NOTFOUND = 404,
	METHNOTALLOWED = 405,
	NOTACCEPTABLE = 406,
	TOOLARGE = 413,
	INTERNALSERVERROR = 500,
};