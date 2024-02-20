#include "../include/parsing.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>

// client_max_body_size 1B; = 1
// client_max_body_size 1K; = 1000
// client_max_body_size 1M; = 1 000 000
// client_max_body_size 1G; = 1 000 000 000

//Quand je vais creer le server, verifier que j'ai rien en dehors du scope du server, donc si j'ai un host random 
//en dehors, je quitte le programme ou je prend pas en compte la ligne

void parsing::assignConfigFile(){
	string line;
	std::ifstream file;

	file.open(pathConfigFile);
	for (size_t i = 0; i < verifLine.size(); i++){
		std::getline(file, line);
		if (verifLine[i] == 0){
			createServer(line, file, &i);
		}
		// cout << "line " << i + 1 << "	" << line << " : " << verifLine[i] << endl;
	}
	// rout->setRoot(const std::string &root);
	// rout->setIndex(const std::string &index);
	// rout->addAllowedMethod(const std::string &method);
	// rout->addErrorPage(const int errorNumber, std::string pathToError);
	//si location
	// Location *loc = new Location();
	// loc->addAllowedMethod(const std::string &method);
	// loc->addErrorPage(const int errorNumber, std::string pathToError);
	// loc->setIndex(const std::string &index);
	// loc->setRoot(const std::string &root);
	// rout->addLocation(const std::string &key, Location *loc);
	// Server *serv = new Server();
}

void	parsing::createServer(string &line, std::ifstream &file, size_t *i){
	if (line.find("server") != 0 || line.find("server_name") != string::npos)
		return ;
	uint16_t port = 0;
	const char *host = NULL;
	string name;
	unsigned long clientBodySize = 0;
	Router *router = new Router();
	(*i)++;
	while (*i < verifLine.size()){
		std::getline(file, line);
		// cout << line << endl;
		if (verifLine[*i] == 0){
			if (port == 0 && line.find("listen") == 1)
				port = assignPort(line);
			if (host == NULL && line.find("host") == 1){
				string temp = assignHost(line);
				host = temp.c_str();
			}
			if (name.empty() && line.find("server_name") == 1)
				name = assignServerName(line);
			if (clientBodySize == 0 && line.find("client_max_body_size") == 1)
				clientBodySize = assignMaxBody(line);
			assignRoot(line, *router);
			assignIndex(line, *router);
			// assignErrorPage(line, *router);
			// assignReturn(line, *router);
			assignLocation(line, file, i, *router);
		}
		if (line.find("}") == 0){
			cout << "SERVER TERMINER" << endl;
			break ;
		}
		(*i)++;
	}
	cout << "Port is " << port << endl;
	cout << "Host is " << host << endl;
	cout << "ServerName is " << name << endl;
	cout << "ClientMaxBodySize is " << clientBodySize << endl;

	// if (anyAreNull)
		// setDefault
	// webserv.addNewServer(port, host, name, router, clientBodySize);
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

unsigned long	parsing::assignMaxBody(const string &line){
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
	return CMBS;
}

void parsing::assignRoot(const string &line, Router &rout){
	if (line.find("root") != 1)
		return ;
	string tempLine = line;
	tempLine.erase(std::remove_if(tempLine.begin(), tempLine.end(), ::isspace), tempLine.end());
	tempLine.erase(0, 4); tempLine.erase(tempLine.size() - 1, tempLine.size());
	rout.setRoot(tempLine);
}

void parsing::assignIndex(const string &line, Router &rout){
	if (line.find("index") != 1)
		return ;
	string tempLine = line;
	tempLine.erase(std::remove_if(tempLine.begin(), tempLine.end(), ::isspace), tempLine.end());
	tempLine.erase(0, 5); tempLine.erase(tempLine.size() - 1, tempLine.size());
	rout.setIndex(tempLine);
}

// void parsing::assignErrorPage(const string &line, Router &rout){
	
// }

// void parsing::assignReturn(const string &line, Router &rout){
// }

void	parsing::assignLocation(string &line, std::ifstream &file, size_t *i, Router &rout){
	if (line.find("location") != 1)
		return ;
	Location	loc;
	(*i)++;
	while (*i < verifLine.size()){
		std::getline(file, line);
		// cout << line << endl;
		if (verifLine[*i] == 0){
			assignAllowedMethods(line, loc);
			assignIndex(line, loc);
			assignRoot(line, loc);
			// assignMaxBody(line, loc);
			// assignReturn(line, loc);
			// assignErrorPage(line, loc);
		}
		if (line.find("}") == 1){
			cout << "LOCATION TERMINER" << endl;
			break ;
		}
		(*i)++;
	}
	// if (anyAreNull)
		// setDefault
	rout.addLocation("key", &loc);
}

void parsing::assignIndex(const string &line, Location &loc){
	if (line.find("index") != 1)
		return ;
	string tempLine = line;
	tempLine.erase(std::remove_if(tempLine.begin(), tempLine.end(), ::isspace), tempLine.end());
	tempLine.erase(0, 5); tempLine.erase(tempLine.size() - 1, tempLine.size());
	loc.setIndex(tempLine);
}

void parsing::assignRoot(const string &line, Location &loc){
	if (line.find("root") != 1)
		return ;
	string tempLine = line;
	tempLine.erase(std::remove_if(tempLine.begin(), tempLine.end(), ::isspace), tempLine.end());
	tempLine.erase(0, 4); tempLine.erase(tempLine.size() - 1, tempLine.size());
	loc.setRoot(tempLine);
}

void	parsing::assignAllowedMethods(const string &line, Location &loc){
	if (line.find("allowedMethods") != 2)
		return ;
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
		else if (res[i] == "PUT")
			loc.addAllowedMethod(res[i]);
		else if (res[i] == "POST")
			loc.addAllowedMethod(res[i]);
		else if (res[i] == "PATCH")
			loc.addAllowedMethod(res[i]);
		else if (res[i] == "DELETE")
			loc.addAllowedMethod(res[i]);
		else if (res[i] == "CONNECT")
			loc.addAllowedMethod(res[i]);
		else if (res[i] == "OPTIONS")
			loc.addAllowedMethod(res[i]);
		else if (res[i] == "TRACE")
			loc.addAllowedMethod(res[i]);
	}
}

// void parsing::assignErrorPage(const string &line, Location &loc){
// }

// void parsing::assignReturn(const string &line, Location &loc){
// }