#include "../include/parsing.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <string>
#include <type_traits>
#include <vector>

// client_max_body_size 1B; = 1
// client_max_body_size 1K; = 1000
// client_max_body_size 1M; = 1 000 000
// client_max_body_size 1G; = 1 000 000 000

//Quand je vais creer le server, verifier que j'ai rien en dehors du scope du server, donc si j'ai un host random 
//en dehors, je quitte le programme ou je prend pas en compte la ligne

void	parsing::setDefault(uint16_t *_port, const char *_host, string *_name, Router &rout){
	if (*_port == 0)
		*_port = DEFAULTLISTEN;
	if (_host == NULL)
		_host = DEFAULTHOST;
	if (*_name == "")
		*_name = DEFAULTSERVERNAME;
	if (rout.getClientMaxBodySize() == -1)
		rout.setClientMaxBodySize(DEFAULTMAXBODY);
	if (rout.getRoot() == "")
		rout.setRoot(DEFAULTROOT);
	if (rout.getIndex() == "")
		rout.setIndex(DEFAULTINDEX);
}

void parsing::assignConfigFile(){
	string line;
	std::ifstream file;

	file.open(pathConfigFile);
	for (size_t i = 0; i < verifLine.size(); i++){
		std::getline(file, line);
		if (verifLine[i] == 0){
			createServer(line, file, &i);
		}
	}
}

void	parsing::createServer(string &line, std::ifstream &file, size_t *i){
	if (line.find("server") != 0 || line.find("server_name") != string::npos)
		return ;
	uint16_t port = 0;
	const char *host = NULL;
	string name;
	Router *router = new Router();
	(*i)++;
	while (*i < verifLine.size()){
		std::getline(file, line);
		// cout << line << endl;
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
			// cout << "SERVER TERMINER" << endl;
			break ;
		}
		(*i)++;
	}
	setDefault(&port, host, &name, *router);
	// cout << "Port is " << port << endl;
	// cout << "Host is " << host << endl;
	// cout << "ServerName is " << name << endl;
	// webserv.addNewServer(port, host, name, router);
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
	for (size_t i = 1; i < split.size(); i++) {
		if (containsNonDigit(split[i]) == false && split[i].length() > 0){
			rout.addErrorPage(std::stoi(split[i]), split[split.size() - 1]);
			// cout << split[i] << " a été assigner a " << split[split.size() - 1] << endl;
		}
	}
}

// void	parsing::assignReturn(const string &line, Router &rout){
// 	std::vector<string> split = splitString(line, ' ');
// 	for (size_t i = 1; i < split.size(); i++) {
// 		if (containsNonDigit(split[i]) == false && split[i].length() > 0){
// 			// rout. = true;
// 			// rout.setreturncode = split[i];
// 			// rout.setreturnlocation = split[split.size() - 1];
// 			// return ;
// 			cout << split[i] << " a été assigner a " << split[split.size() - 1] << endl;
// 		}
// 	}
// }



//----------------------------LOCATION---------------------------------

void	parsing::assignLocation(string &line, std::ifstream &file, size_t *i, Router &rout){
	string	name = line.substr(line.rfind(" "), (line.length() - line.rfind(" ")) - 1);
	Location	loc(name);
	(*i)++;
	while (*i < verifLine.size()){
		std::getline(file, line);
		// cout << line << endl;
		if (verifLine[*i] == 0){
			if (findFirstWord(line) == "allowedMethods")
				assignAllowedMethods(line, loc);
			if (findFirstWord(line) == "index")
				assignIndex(line, loc);
			if (findFirstWord(line) == "root")
				assignRoot(line, loc);
			if (findFirstWord(line) == "client_max_body_size")
				assignMaxBody(line, loc);
			if (findFirstWord(line) == "return")
				assignReturn(line, loc);
			if (findFirstWord(line) == "error_page")
				assignErrorPage(line, loc);
			if (findFirstWord(line) == "upload_enable")
				assignUploadBool(line, loc);
			if (findFirstWord(line) == "upload_store")
				assignUploadStore(line, loc);
			if (findFirstWord(line) == "autoindex")
				assignAutoIndex(line, loc);
		}
		if (line.find("}") == 1){
			// cout << "LOCATION TERMINER\n" << endl;
			break ;
		}
		(*i)++;
	}
	if (loc.getUploadStore() == "")
		loc.setUploadStore(DEFAULTSTORE);
	rout.addLocation(loc.getName(), &loc);
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
			// cout << split[i] << " a été assigner a " << split[split.size() - 1] << endl;
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
	std::vector<string> split = splitString(line, ' ');
	for (size_t i = 1; i < split.size(); i++) {
		if (containsNonDigit(split[i]) == false && split[i].length() > 0){
			loc.setRedirection(true);
			loc.setRedirectionCode(std::stoi(split[i]));
			loc.setRedirectionLocation(split[split.size() - 1]);
			// cout << split[i] << " a été assigner a " << split[split.size() - 1] << endl;
			return ;
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