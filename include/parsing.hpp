#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <sys/stat.h>
#include "color.h"
#include "utils.hpp"

#define DEFAULTHOST "127.0.0.1"
#define DEFAULTLISTEN "8080"
#define DEFAULTSERVERNAME "default"
#define DEFAULTERRORPAGE "404 /Users/rapelcha/Desktop/Webserv/www/error.html"
#define DEFAULTMETHODS "GET, PUT, POST, PATCH, DELETE, CONNECT, OPTIONS, TRACE"
#define DEFAULTLOCATION DEFAULTMETHODS
#define DEFAILTBODYSIZE "1M"

enum flags{
	CORRECT,
	WRONGPATH,
	DIR,
	WRONGEXT,
	SERVERPARS,
	HOST_ERR,
	LISTEN_ERR,
	SERVERNAME_ERR,
	ROOT_ERR,
	DEFAULT_FILE_ERR,
	ERROR_PAGE_ERR,
	LOCATION_ERR,
	ALLOWEDMETHODS_ERR,
	RETURN_ERR,
	CLIENT_MAX_BODY_SIZE_ERR
};

using std::string;
using std::cout;
using std::endl;

class parsing{
	public:
		parsing(string path);
		bool parseConfigFile();
		~parsing();

	private:
		string	pathConfigFile;
		std::ifstream	configFile;
		bool	defaultIfError;
		void	error(int errorCode);
		bool	checkFile();
		int		checkDefault(std::string const &line);
		int		checkServer(std::string const &line);
		int		checkHost(std::string const &line);
		int		checkListen(std::string const &line);
};
