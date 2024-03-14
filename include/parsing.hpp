#pragma once

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <vector>
#include "Webserv.hpp"
#include "color.h"
#include "../include/Server.hpp"
#include "../include/Router.hpp"
#include "../include/Location.hpp"
#include "utils.hpp"

// Router
#define DEFAULTROOT "./www"
#define DEFAULTINDEX "index.html"
#define DEFAULTMAXBODY 1000000
#define LOCATIONIFNONE "/"

// Server
#define DEFAULTSERVERNAME "default"
#define DEFAULTHOST "127.0.0.1"
#define DEFAULTLISTEN 8080

// Location
#define DEFAULTMETHODS "GET POST DELETE"
#define DEFAULTSTORE "/uploads"

enum verifFlags{
	OKPARS,
	SWITCH,
	DONT
};

using std::string;
using std::cout;
using std::endl;

class Webserv;

class parsing{
	public:
		parsing(string path);
		bool parseConfigFile();
		void assignConfigFile(Webserv *webserv);
		~parsing();

	private:
		string				pathConfigFile;
		std::vector<int>	verifLine;
		std::ifstream		configFile;
		bool				defaultIfError;
		// Analyser ConfigFile
		bool				checkFile();
		bool				checkValid(string const &line);
		int					isThisTheEnd(string const &line, bool insideLocation);
		string				findFirstWord(string line);
			//check les lignes
		void				checkDefault(string const &line, unsigned int nbLine);
		bool				checkServer(string &line, unsigned int *nbLine);
		bool				checkHost(string const &line, unsigned int nbLine);
		bool				checkListen(string const &line, unsigned int nbLine);
		bool				checkServerName(string const &line, unsigned int nbLine);
		bool				checkRoot(string const &line, unsigned int nbLine);
		bool				checkIndex(string const &line, unsigned int nbLine);
		bool				checkErrorPage(string const &line, unsigned int nbLine);
		bool				checkClientMaxBodySize(string const &line, unsigned int nbLine);
		bool				checkReturns(string const &line, unsigned int nbLine);
				// Dans location
		bool					checkLocation(string &line, unsigned int *nbLine);
		void					checkIndexLocation(string const &line, unsigned int nbLine);
		void					checkRootLocation(string const &line, unsigned int nbLine);
		void					checkAllowedMethods(string &line, unsigned int nbLine);
		void					checkErrorPageLocation(string const &line, unsigned int nbLine);
		void					checkReturnsLocation(string const &line, unsigned int nbLine);
		void					checkUploadEnable(string const &line, unsigned int nbLine);
		void					checkUploadStore(string const &line, unsigned int nbLine);
		void					checkClientMaxBodySizeLocation(string const &line, unsigned int nbLine);
		void					checkAutoIndex(const string &line, unsigned int nbline);
		void					checkUseCGI(string const &line, unsigned int nbLine);
		// Assignation
		void				setDefault(uint16_t *_port, const char **_host, string *_name, Router &rout);
			//Server
		void				createServer(string &line, std::ifstream &file, size_t *i, Webserv *webserv);
		uint16_t			assignPort(const string &line);
		string				assignHost(const string &line);
		string				assignServerName(const string &line);
			//Router
		void				assignMaxBody(const string &line, Router &rout);
		void				assignRoot(const string &line, Router &rout);
		void				assignIndex(const string &line, Router &rout);
		void				assignErrorPage(const string &line, Router &rout);
		void				assignReturn(const string &line, Router &rout);
			//Location
		void				assignLocation(string &line, std::ifstream &file, size_t *i, Router &rout);
		void				assignAllowedMethods(const string &line, Location &loc);
		void				assignIndex(const string &line, Location &loc);
		void				assignRoot(const string &line, Location &loc);
		void				assignReturn(const string &line, Location &loc);
		void				assignErrorPage(const string &line, Location &loc);
		void				assignMaxBody(const string &line, Location &loc);
		void				assignUploadBool(const string &line, Location &loc);
		void				assignUploadStore(const string &line, Location &loc);
		void				assignAutoIndex(const string &line, Location &loc);
		void				assignUseCGI(const string &line, Location &loc);
};
