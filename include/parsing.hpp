#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <vector>
#include "color.h"
#include "utils.hpp"

#define DEFAULTHOST "127.0.0.1"
#define DEFAULTLISTEN "8080"
#define DEFAULTSERVERNAME "default"
#define DEFAULTERRORPAGE "404 /Users/rapelcha/Desktop/Webserv/www/error.html"
#define DEFAULTMETHODS "GET, PUT, POST, PATCH, DELETE, CONNECT, OPTIONS, TRACE"
#define DEFAULTLOCATION DEFAULTMETHODS
#define DEFAILTBODYSIZE "1M"

enum verifFlags{
	OKPARS,
	SWITCH,
	DONT
};

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
	ERROR_PAGE_ERR,
	LOCATION_ERR,
	ALLOWEDMETHODS_ERR,
	RETURN_ERR,
	CLIENT_MAX_BODY_SIZE_ERR,
	INDEX_ERR
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
		string				pathConfigFile;
		std::vector<int>	verifLine;
		std::ifstream		configFile;
		bool				defaultIfError;
		void				error(int errorCode);
		bool				checkFile();
		int					checkValid(string const &line);
		int					isThisTheEnd(string const &line);
		int					checkDefault(string const &line);
		int					checkServer(string const &line);
		int					checkHost(string const &line);
		int					checkListen(string const &line);
		int					checkServerName(string const &line);
		int					checkRoot(string const &line, int inden);
		int					checkIndex(string const &line, int inden);
		int					checkErrorPage(string const &line, int inden);
		int					checkClientMaxBodySize(string const &line);
		int					checkReturns(string const &line);
		// Dans location
		int					checkLocation(string &line);
		int					checkIndexLocation(string const &line);
		int					checkRootLocation(string const &line);
		int					checkAllowedMethods(string const &line);
		int					checkErrorPageLocation(string const &line);
		int					checkReturnsLocation(string const &line);
};
