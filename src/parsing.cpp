#include "../include/parsing.hpp"

parsing::parsing(string path): pathConfigFile(path){
	defaultIfError = false;
}

bool	parsing::parseConfigFile(){
	int	ret;
	string line;
	int	nbLine = 0;
	int serverLineCheck;

	writeTimestamp(PURPLE, "Parsing is starting...");
	if (checkFile() == false){
		return false;
	}
	while (std::getline(configFile, line)){
		if (nbLine == 0)
			checkDefault(line);
		if (checkServer(line) == -2)
			return false;
		if (checkHost(line) == -2)
			return false;
		checkListen(line);
		nbLine++;
	}
	return true;
}

int	parsing::checkHost(std::string const &line){

	if (line.find("host") == string::npos)
		return -1;
	if (checkIdentationParsing("host", 1, line, false, defaultIfError, "host line") == false)
		return -2;
	
	writeTimestamp(GREEN, "Host is ok!");
	return true;
}

int	parsing::checkListen(std::string const &line){
	if (line.find("listen") == string::npos)
		return -1;
	writeTimestamp(GREEN, "Listen is ok!");
	return true;
}

int	parsing::checkServer(std::string const &line){
	// writeTimestamp(YELLOW, "In checkServer: " + line);
	if (line.find("server") == string::npos || line.find("server_name") != string::npos)
		return -1;
	if (line.find("server") > 0){
		writeTimestamp(RED, "The server line mustn\'t have indentation!");
		return -2;
	}
	if (line.find("server") != string::npos && line.find("{") != string::npos){
		if (isThereSomethingInMyString(line, "server", "{", false) == false){
			writeTimestamp(GREEN, "Server is being created...");
		}
		else{
			return -2;
		}
	}
	else if (line.find("server") != string::npos && line.find("{") == string::npos){
		writeTimestamp(RED, "Error in the server line, you didnt put the \"{\" at the end!");
		return -2;
	}
	return CORRECT;
}

int	parsing::checkDefault(std::string const &line){
	string begin = "acceptDefault";
	string end;
	int	ret = true;

	if ((line.find("acceptDefault") > 0 || line.find("acceptDefault") == string::npos)){
		writeTimestamp(YELLOW, "You choose that the program will close if they're is an error in the ConfigFile");
		writeTimestamp(YELLOW, "If you don't want that, you must include a the beginning of the ConfigFile : \"acceptDefault true\"");
		return -1;
	}
	if (line.find("acceptDefault ") < string::npos || line.find("acceptDefault	") < string::npos){
		if (line.find("true;") < string::npos && line.find("true;") + 5 == line.length()){
			end = "true;";
			defaultIfError = true;
		}
		else if (line.find("false;") < string::npos && line.find("false;") + 6 == line.length()){
			end = "false;";
			defaultIfError = false;
		}
	}
	if (begin.length() > 0 && end.length() > 0){
		if (isThereSomethingInMyString(line, begin, end, defaultIfError) == true){
			return -1;
		}
	}
	else if (end.length() <= 0){
		ret = false;
	}
	if (end == "false;")
		ret = false;
	if (ret == true)
		writeTimestamp(GREEN, "You choose that the program will choose default configuration if the ConfigFile has some error in it!");
	else{
		writeTimestamp(YELLOW, "You choose that the program will close if they're is an error in the ConfigFile");
		writeTimestamp(YELLOW, "If you don't want that, you must include a the beginning of the ConfigFile : \"acceptDefault true\"");
	}
	return ret;
}

bool	parsing::checkFile(){
	string			line;
	struct stat		fileStat;

	if (stat(pathConfigFile.c_str(), &fileStat) == 0){
		if (S_ISDIR(fileStat.st_mode) == true){
			error(DIR);
			return DIR;
		}
	}
	else{
		error(WRONGPATH);
		return WRONGPATH;
	}
	if (pathConfigFile.find(".cfg") == string::npos){
		error(WRONGEXT);
		return WRONGEXT;
	}
	configFile.open(pathConfigFile);
	writeTimestamp(GREEN, "Path correct!");
	return true;
}

void	parsing::error(int errorCode){
	switch (errorCode){
		case 1:
			writeTimestamp(RED, "The ConfigFile you entered doesn't exist");
			break;
		case 2:
			writeTimestamp(RED, "The \"File\" isn't one, it's a directory");
			break;
		case 3:
			writeTimestamp(RED, "The ConfigFile you entered doesn't have the right extension, the one needed is : \".cfg\"");
			break;
		case 4:
			writeTimestamp(RED, "The server is not valid and you choose to not acceptDefault, skill issue");
			break;
	}
}

parsing::~parsing(){
}