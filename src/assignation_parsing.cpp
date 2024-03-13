#include "../include/parsing.hpp"
#include <algorithm>
#include <string>
#include <vector>

void	parsing::setDefault(uint16_t *_port, const char **_host, string *_name, Router &rout){
	if (*_port == 0)
		*_port = DEFAULTLISTEN;
	if (*_host == NULL)
		*_host = DEFAULTHOST;
	if (*_name == "")
		*_name = DEFAULTSERVERNAME;

	//router
	if (rout.getRoot(NULL) == "")
		rout.setRoot(DEFAULTROOT);
	if (rout.getClientMaxBodySizeParsing() == -1)
		rout.setClientMaxBodySize(DEFAULTMAXBODY);
	if (rout.getIndex() == "")
		rout.setIndex(DEFAULTINDEX);
	if (rout.getLocationParsing() == "" || rout.getLocationParsing().find("	/	") == string::npos){
		Location *loc = new Location("/");
		loc->addAllowedMethod("GET");
		loc->addAllowedMethod("POST");
		loc->addAllowedMethod("DELETE");
		loc->setUploadStore(DEFAULTSTORE);
		rout.addLocation(loc->getName(), loc);
	}

	// Error code
	if (rout.getErrorForParsing(200) == "")
		rout.addErrorPage(200, "./www/errors/200.html");
	if (rout.getErrorForParsing(201) == "")
		rout.addErrorPage(201, "./www/errors/201.html");
	if (rout.getErrorForParsing(202) == "")
		rout.addErrorPage(202, "./www/errors/202.html");
	if (rout.getErrorForParsing(204) == "")
		rout.addErrorPage(204, "./www/errors/204.html");
	if (rout.getErrorForParsing(205) == "")
		rout.addErrorPage(205, "./www/errors/205.html");
	if (rout.getErrorForParsing(206) == "")
		rout.addErrorPage(206, "./www/errors/206.html");
	if (rout.getErrorForParsing(300) == "")
		rout.addErrorPage(300, "./www/errors/300.html");
	if (rout.getErrorForParsing(301) == "")
		rout.addErrorPage(301, "./www/errors/301.html");
	if (rout.getErrorForParsing(302) == "")
		rout.addErrorPage(302, "./www/errors/302.html");
	if (rout.getErrorForParsing(303) == "")
		rout.addErrorPage(303, "./www/errors/303.html");
	if (rout.getErrorForParsing(304) == "")
		rout.addErrorPage(304, "./www/errors/304.html");
	if (rout.getErrorForParsing(305) == "")
		rout.addErrorPage(305, "./www/errors/305.html");
	if (rout.getErrorForParsing(307) == "")
		rout.addErrorPage(307, "./www/errors/307.html");
	if (rout.getErrorForParsing(400) == "")
		rout.addErrorPage(400, "./www/errors/400.html");
	if (rout.getErrorForParsing(401) == "")
		rout.addErrorPage(401, "./www/errors/401.html");
	if (rout.getErrorForParsing(403) == "")
		rout.addErrorPage(403, "./www/errors/403.html");
	if (rout.getErrorForParsing(404) == "")
		rout.addErrorPage(404, "./www/errors/404.html");
	if (rout.getErrorForParsing(405) == "")
		rout.addErrorPage(405, "./www/errors/405.html");
	if (rout.getErrorForParsing(406) == "")
		rout.addErrorPage(406, "./www/errors/406.html");
	if (rout.getErrorForParsing(413) == "")
		rout.addErrorPage(413, "./www/errors/413.html");
	if (rout.getErrorForParsing(500) == "")
		rout.addErrorPage(500, "./www/errors/500.html");
}

void parsing::assignConfigFile(Webserv *webserv){
	string line;
	std::ifstream file;

	file.open(pathConfigFile);
	for (size_t i = 0; i < verifLine.size(); i++){
		std::getline(file, line);
		if (verifLine[i] == 0){
			createServer(line, file, &i, webserv);
		}
	}
	bool flag = false;
	file.close();
	file.open(pathConfigFile);
	while (std::getline(file, line)){
		if (line.find("server") == 0 && line.find("server_name") == string::npos)
			flag = true;
	}
	if (verifLine.size() == 0 || flag == false){
		uint16_t _port = 0;
		const char *_host = NULL;
		string _name = "";
		Router *router = new Router();
		setDefault(&_port, &_host, &_name, *router);
		webserv->addNewServer(_port, _host, _name, router);
	}
}

void	parsing::createServer(string &line, std::ifstream &file, size_t *i, Webserv *webserv){
	if (line.find("server") != 0 || line.find("server_name") != string::npos)
		return ;
	uint16_t port = 0;
	const char *host = NULL;
	string name;
	Router *router = new Router();
	(*i)++;
	while (*i < verifLine.size()){
		std::getline(file, line);
		if (verifLine[*i] == 0){
			if (port == 0 && findFirstWord(line) == "listen"){
				port = assignPort(line);
			}
			if (host == NULL && findFirstWord(line) == "host"){
				string temp = assignHost(line);
				host = temp.c_str();
			}
			if (name.empty() && findFirstWord(line) == "server_name")
				name = assignServerName(line);
			if (findFirstWord(line) == "client_max_body_size")
				assignMaxBody(line, *router);
			if (findFirstWord(line) == "root")
				assignRoot(line, *router);
			if (findFirstWord(line) == "index")
				assignIndex(line, *router);
			if (findFirstWord(line) == "error_page")
				assignErrorPage(line, *router);
			if (findFirstWord(line) == "location")
				assignLocation(line, file, i, *router);
		}
		if (line.find("}") == 0){
			break ;
		}
		(*i)++;
	}
	setDefault(&port, &host, &name, *router);
	webserv->addNewServer(port, host, name, router);
}

uint16_t	parsing::assignPort(const string &line){
	uint16_t	port = 0;
	string		tempLine = line;
	tempLine.erase(std::remove_if(tempLine.begin(), tempLine.end(), ::isspace), tempLine.end());
	tempLine.erase(0, 6); tempLine.erase(tempLine.size() - 1, tempLine.size());
	port = static_cast<uint16_t>(stoi(tempLine));
	return port;
}

string	parsing::assignHost(const string &line){
	string	tempLine = line;
	tempLine.erase(std::remove_if(tempLine.begin(), tempLine.end(), ::isspace), tempLine.end());
	tempLine.erase(0, 4); tempLine.erase(tempLine.size() - 1, tempLine.size());
	return tempLine;
}

string	parsing::assignServerName(const string &line){
	string	serverName = line;
	serverName.erase(std::remove_if(serverName.begin(), serverName.end(), ::isspace), serverName.end());
	serverName.erase(0, 11); serverName.erase(serverName.size() - 1, serverName.size());
	return serverName;
}

void	parsing::assignMaxBody(const string &line, Router &rout){
	unsigned long	CMBS = 0;
	string tempLine = line;
	tempLine.erase(std::remove_if(tempLine.begin(), tempLine.end(), ::isspace), tempLine.end());
	tempLine.erase(0, 20); tempLine.erase(tempLine.size() - 1, tempLine.size());
	CMBS = static_cast<unsigned long>(stoi(tempLine));
	if (tempLine[tempLine.size() - 1] == 'B'){
		CMBS *= 1;
	}if (tempLine[tempLine.size() - 1] == 'K'){
		CMBS *= 1000;
	}if (tempLine[tempLine.size() - 1] == 'M'){
		CMBS *= 1000000;
	}if (tempLine[tempLine.size() - 1] == 'G'){
		CMBS *= 1000000000;
	}
	rout.setClientMaxBodySize(CMBS);
}

void parsing::assignRoot(const string &line, Router &rout){
	string tempLine = line;
	tempLine.erase(std::remove_if(tempLine.begin(), tempLine.end(), ::isspace), tempLine.end());
	tempLine.erase(0, 4); tempLine.erase(tempLine.size() - 1, tempLine.size());
	rout.setRoot(tempLine);
}

void parsing::assignIndex(const string &line, Router &rout){
	string tempLine = line;
	tempLine.erase(std::remove_if(tempLine.begin(), tempLine.end(), ::isspace), tempLine.end());
	tempLine.erase(0, 5); tempLine.erase(tempLine.size() - 1, tempLine.size());
	rout.setIndex(tempLine);
}

void parsing::assignErrorPage(const string &line, Router &rout){
	std::vector<string> split = splitString(line, ' ');
	split[split.size() - 1].erase(split[split.size() - 1].length() - 1, split[split.size() - 1].length());
	for (size_t i = 1; i < split.size(); i++) {
		if (containsNonDigit(split[i]) == false && split[i].length() > 0){
			rout.addErrorPage(std::stoi(split[i]), split[split.size() - 1]);
		}
	}
}

//----------------------------LOCATION---------------------------------

void	parsing::assignLocation(string &line, std::ifstream &file, size_t *i, Router &rout){
	string	name = line.substr(line.rfind(" "), (line.length() - line.rfind(" ")) - 1); name.erase(std::remove_if(name.begin(), name.end(), ::isspace), name.end());
	Location	*loc = new Location(name);
	(*i)++;
	while (*i < verifLine.size()){
		std::getline(file, line);
		if (verifLine[*i] == 0){
			if (findFirstWord(line) == "allowedMethods")
				assignAllowedMethods(line, *loc);
			if (findFirstWord(line) == "index")
				assignIndex(line, *loc);
			if (findFirstWord(line) == "root")
				assignRoot(line, *loc);
			if (findFirstWord(line) == "client_max_body_size")
				assignMaxBody(line, *loc);
			if (findFirstWord(line) == "return")
				assignReturn(line, *loc);
			if (findFirstWord(line) == "error_page")
				assignErrorPage(line, *loc);
			if (findFirstWord(line) == "upload_enable")
				assignUploadBool(line, *loc);
			if (findFirstWord(line) == "upload_store")
				assignUploadStore(line, *loc);
			if (findFirstWord(line) == "autoindex")
				assignAutoIndex(line, *loc);
		}
		if (line.find("}") == 1){
			break ;
		}
		(*i)++;
	}
	if (loc->getUploadStore() == "")
		loc->setUploadStore(DEFAULTSTORE);
	if (loc->getAllowedMethods() == ""){
		loc->addAllowedMethod("GET");
		loc->addAllowedMethod("POST");
		loc->addAllowedMethod("DELETE");
	}
	rout.addLocation(loc->getName(), loc);
}

void parsing::assignIndex(const string &line, Location &loc){
	string tempLine = line;
	tempLine.erase(std::remove_if(tempLine.begin(), tempLine.end(), ::isspace), tempLine.end());
	tempLine.erase(0, 5); tempLine.erase(tempLine.size() - 1, tempLine.size());
	loc.setIndex(tempLine);
}

void parsing::assignRoot(const string &line, Location &loc){
	string tempLine = line;
	tempLine.erase(std::remove_if(tempLine.begin(), tempLine.end(), ::isspace), tempLine.end());
	tempLine.erase(0, 4); tempLine.erase(tempLine.size() - 1, tempLine.size());
	loc.setRoot(tempLine);
}

void	parsing::assignAllowedMethods(const string &line, Location &loc){
	std::vector<std::string> res = splitString(line, ',');
	for (size_t i = 0 ; i < res.size(); i++){
		if (i == 0)
			res[i].erase(0, 17);
		else if (i < res.size() - 1)
			res[i].erase(0, 1);
		else if (i < res.size()){
			res[i].erase(0, 1);
			res[i].erase(res[i].size() - 1, res[i].size());
		}
	}
	for (size_t i = 0 ; i < res.size(); i++){
		if (res[i] == "GET")
			loc.addAllowedMethod(res[i]);
		else if (res[i] == "POST")
			loc.addAllowedMethod(res[i]);
		else if (res[i] == "DELETE")
			loc.addAllowedMethod(res[i]);
	}
}

void	parsing::assignErrorPage(const string &line, Location &loc){
	std::vector<string> split = splitString(line, ' ');
	for (size_t i = 1; i < split.size(); i++) {
		if (containsNonDigit(split[i]) == false && split[i].length() > 0){
			loc.addErrorPage(std::stoi(split[i]), split[split.size() - 1]);
		}
	}
}

void	parsing::assignMaxBody(const string &line, Location &loc){
	unsigned long	CMBS = 0;
	string tempLine = line;
	tempLine.erase(std::remove_if(tempLine.begin(), tempLine.end(), ::isspace), tempLine.end());
	tempLine.erase(0, 20); tempLine.erase(tempLine.size() - 1, tempLine.size());
	CMBS = static_cast<unsigned long>(stoi(tempLine));
	if (tempLine[tempLine.size() - 1] == 'B'){
		CMBS *= 1;
	}if (tempLine[tempLine.size() - 1] == 'K'){
		CMBS *= 1000;
	}if (tempLine[tempLine.size() - 1] == 'M'){
		CMBS *= 1000000;
	}if (tempLine[tempLine.size() - 1] == 'G'){
		CMBS *= 1000000000;
	}
	loc.setClientMaxBodySize(CMBS);
}

void parsing::assignReturn(const string &line, Location &loc){
	string tempLine = line;
	tempLine.erase(0, 9);
	loc.setRedirection(true);
	std::vector<string> res = splitString(tempLine, ' ');
	for (size_t i = 0; i < res.size(); i++){
		res[i].erase(std::remove_if(res[i].begin(), res[i].end(), ::isspace), res[i].end());
		if (res[i].find(";") != string::npos)
			res[i].erase(res[i].length() - 1, res[i].length());
	}
	for (size_t i = 0; i < res.size(); i++){
		if (!res[i].empty()){
			if (containsNonDigit(res[i]) == false){
				loc.setRedirectionCode(std::stoi(res[i]));
			}else if (res[i].find("\"") < string::npos){
				res[i].erase(std::remove(res[i].begin(), res[i].end(), '"'), res[i].end());
				loc.setRedirectionLocation(res[i], true);
			}else {
				loc.setRedirectionLocation(res[i], false);
			}
		}
	}
}

void	parsing::assignUploadBool(const string &line, Location &loc){
	int start = line.rfind(" ");
	string str = line.substr(start + 1, line.length() - start);
	if (str == "true;")
		loc.setUploadEnable(true);
	else if (str == "false;")
		loc.setUploadEnable(false);
}

void	parsing::assignUploadStore(const string &line, Location &loc){
	int start = line.rfind(" ");
	string str = line.substr(start + 1, (line.length() - 2) - start);
	loc.setUploadStore(str);
}

void	parsing::assignAutoIndex(const string &line, Location &loc){
	int start = line.rfind(" ");
	string str = line.substr(start + 1, line.length() - start);
	if (str == "true;")
		loc.setAutoIndex(true);
	else if (str == "false;")
		loc.setAutoIndex(false);
}