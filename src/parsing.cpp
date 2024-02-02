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
		// cout << line << endl;
		if (nbLine == 0)
			checkDefault(line);
		if (checkServer(line) == -2)
			return (error(SERVERPARS), false);
		checkLocation(line);
		if (checkHost(line) == -2)
			return (error(HOST_ERR), false);
		if (checkListen(line) == -2)
			return (error(LISTEN_ERR), false);
		if (checkServerName(line) == -2)
			return (error(SERVERNAME_ERR), false);
		if (checkRoot(line, 1) == -2)
			return (error(ROOT_ERR), false);
		if (checkIndex(line, 1) == -2)
			return (error(INDEX_ERR), false);
		if (checkErrorPage(line, 1) == -2)
			return (error(ERROR_PAGE_ERR), false);
		// if (checkClientMaxBodySize(line) == -2)
			// return (error(CLIENT_MAX_BODY_SIZE_ERR), false);
		// if (checkReturns(line, 1) == -2)
			// return (error(RETURN_ERR), false);
		nbLine++;
	}
	return true;
}

int	parsing::checkErrorPage(string const &line, int iden){
	if (line.find("error_page") == string::npos)
		return -1;
	if (line.find(";") != line.length() - 1){
		if (defaultIfError == false){
			writeTimestamp(RED, "You need to have a \";\" at the end of the error_page line");
			return -2;
		}
		else
			writeTimestamp(YELLOW, "You need to have a \";\" at the end of the error_page line, switching to default!");
	}
	if (checkIdentationParsing("error_page", iden, line, defaultIfError, "error_page line") == false)
		return -2;
	if (line.rfind(" ") > line.rfind("	")){
		string ret = retIsThereSomethingInMyString(line, "error_page", &line[line.rfind(" ")], false);
		if (containsNonDigit(ret, defaultIfError) == true){
			if (defaultIfError == false)
				return -2;
		}
	}
	else{
		string ret = retIsThereSomethingInMyString(line, "error_page", &line[line.rfind("	")], false);
		if (containsNonDigit(ret, defaultIfError) == true){
			if (defaultIfError == false)
				return -2;
		}
	}
	writeTimestamp(GREEN, "Error_page is ok!");
	return true;
}

int	parsing::checkIndex(string const &line, int iden){
	if (line.find("index") == string::npos || line.find("index") == 2)
		return -1;
	if (line.find(";") != line.length() - 1){
		if (defaultIfError == false){
			writeTimestamp(RED, "You need to have a \";\" at the end of the index line");
			return -2;
		}
		else
			writeTimestamp(YELLOW, "You need to have a \";\" at the end of the index line, switching to default!");
	}
	if (checkIdentationParsing("index", iden, line, defaultIfError, "index line") == false)
		return -2;
	if (line.rfind(" ") > line.rfind("	")){
		if (isThereSomethingInMyString(line, "index", &line[line.rfind(" ")], defaultIfError) == true && defaultIfError == false)
			return -2;
	}
	else{
		if (isThereSomethingInMyString(line, "index", &line[line.rfind("	")], defaultIfError) == true && defaultIfError == false)
			return -2;
	}
	writeTimestamp(GREEN, "Index is ok!");
	return true;
}

int	parsing::checkRoot(string const &line, int iden){
	if (line.find("root") == string::npos || line.find("root") == 2)
		return -1;
	if (line.find(";") != line.length() - 1){
		if (defaultIfError == false){
			writeTimestamp(RED, "You need to have a \";\" at the end of the root line");
			return -2;
		}
		else
			writeTimestamp(YELLOW, "You need to have a \";\" at the end of the root line, switching to default!");
	}
	if (checkIdentationParsing("root", iden, line, defaultIfError, "root line") == false)
		return -2;
	if (line.rfind(" ") > line.rfind("	")){
		if (isThereSomethingInMyString(line, "root", &line[line.rfind(" ")], defaultIfError) == true && defaultIfError == false)
			return -2;
	}
	else{
		if (isThereSomethingInMyString(line, "root", &line[line.rfind("	")], defaultIfError) == true && defaultIfError == false)
			return -2;
	}
	writeTimestamp(GREEN, "Root is ok!");
	return true;
}

int	parsing::checkServerName(string const &line){
	if (line.find("server_name") == string::npos)
		return -1;
	if (line.find(";") != line.length() - 1){
		if (defaultIfError == false){
			writeTimestamp(RED, "You need to have a \";\" at the end of the server_name line");
			return -2;
		}
		else
			writeTimestamp(YELLOW, "You need to have a \";\" at the end of the server_name line, switching to default!");
	}
	if (checkIdentationParsing("server_name", 1, line, defaultIfError, "server_name line") == false)
		return -2;
	if (line.rfind(" ") > line.rfind("	")){
		if (isThereSomethingInMyString(line, "server_name", &line[line.rfind(" ")], defaultIfError) == true && defaultIfError == false)
			return -2;
	}
	else{
		if (isThereSomethingInMyString(line, "server_name", &line[line.rfind("	")], defaultIfError) == true && defaultIfError == false)
			return -2;
	}
	writeTimestamp(GREEN, "Server_name is ok!");
	return true;
}

int	parsing::checkHost(string const &line){

	if (line.find("host") == string::npos)
		return -1;
	if (line.find(";") != line.length() - 1){
		if (defaultIfError == false){
			writeTimestamp(RED, "You need to have a \";\" at the end of the host line");
			return -2;
		}
		else
			writeTimestamp(YELLOW, "You need to have a \";\" at the end of the host line, switching to default!");
	}
	if (checkIdentationParsing("host", 1, line, defaultIfError, "host line") == false)
		return -2;
	if (line.rfind(" ") > line.rfind("	")){
		if (isThereSomethingInMyString(line, "host", &line[line.rfind(" ")], defaultIfError) == true && defaultIfError == false)
			return -2;
	}
	else{
		if (isThereSomethingInMyString(line, "host", &line[line.rfind("	")], defaultIfError) == true && defaultIfError == false)
			return -2;
	}
	writeTimestamp(GREEN, "Host is ok!");
	return true;
}

int	parsing::checkListen(string const &line){
	if (line.find("listen") == string::npos)
		return -1;
	if (line.find(";") != line.length() - 1){
		if (defaultIfError == false){
			writeTimestamp(RED, "You need to have a \";\" at the end of the listen line");
			return -2;
		}
		else
			writeTimestamp(YELLOW, "You need to have a \";\" at the end of the listen line, switching to default!");
	}
	if (checkIdentationParsing("listen", 1, line, defaultIfError, "listen line") == false)
		return -2;
	if (line.rfind(" ") > line.rfind("	") && line.rfind("	") != string::npos){
		if (isThereSomethingInMyString(line, "listen", &line[line.rfind(" ")], defaultIfError) == true && defaultIfError == false)
			return -2;
		if (containsNonDigit(&line[line.rfind(" ") + 1], defaultIfError) == true)
			return -2;
	}
	else if(line.rfind(" ") < line.rfind("	") && line.rfind(" ") != string::npos){
		if (isThereSomethingInMyString(line, "listen", &line[line.rfind("	")], defaultIfError) == true && defaultIfError == false)
			return -2;
		if (containsNonDigit(&line[line.rfind("	") + 1], defaultIfError) == true)
			return -2;
	}
	writeTimestamp(GREEN, "Listen is ok!");
	return true;
}

int	parsing::checkServer(std::string const &line){
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

int	parsing::checkDefault(string const &line){
	string begin = "acceptDefault";
	string end;
	int	ret = true;

	if ((line.find("acceptDefault") > 0 || line.find("acceptDefault") == string::npos)){
		writeTimestamp(YELLOW, "You choose that the program will close if they're is an error in the ConfigFile");
		writeTimestamp(YELLOW, "If you don't want that, you must include a the beginning of the ConfigFile : \"acceptDefault true\"");
		return -1;
	}
	if (line.find(";") != line.length() - 1){
		if (defaultIfError == false){
			writeTimestamp(RED, "You need to have a \";\" at the end of the acceptDefault line");
			return -2;
		}
		else
			writeTimestamp(YELLOW, "You need to have a \";\" at the end of the acceptDefault line, switching to default!");
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
			writeTimestamp(RED, "The server line is not valid and you choose to not acceptDefault, skill issue");
			break;
		case 5:
			writeTimestamp(RED, "The host line is not valid and you choose to not acceptDefault, skill issue");
			break;
		case 6:
			writeTimestamp(RED, "The listen line is not valid and you choose to not acceptDefault, skill issue");
			break;
		case 7:
			writeTimestamp(RED, "The server_name line is not valid and you choose to not acceptDefault, skill issue");
			break;
		case 8:
			writeTimestamp(RED, "The root line is not valid and you choose to not acceptDefault, skill issue");
			break;
		case 9:
			writeTimestamp(RED, "The error_page line is not valid and you choose to not acceptDefault, skill issue");
			break;
		case 10:
			writeTimestamp(RED, "The location scope is not valid and you choose to not acceptDefault, skill issue");
			break;
		case 11:
			writeTimestamp(RED, "The allowedMethods line is not valid and you choose to not acceptDefault, skill issue");
			break;
		case 12:
			writeTimestamp(RED, "The return line is not valid and you choose to not acceptDefault, skill issue");
			break;
		case 13:
			writeTimestamp(RED, "The client_max_body_size line is not valid and you choose to not acceptDefault, skill issue");
			break;
		case 14:
			writeTimestamp(RED, "The index line is not valid and you choose to not acceptDefault, skill issue");
			break;
	}
}

parsing::~parsing(){
	writeTimestamp(PURPLE, "Parsing done!");
}
