#include "../include/parsing.hpp"
#include <variant>

parsing::parsing(string path): pathConfigFile(path){
	defaultIfError = false;
}

int	parsing::checkValid(string const &line){
	size_t	space = line.rfind(" ");
	string	res;
	if (space == std::string::npos)
		space = 0;
	size_t tabs = line.rfind("	");
	if (tabs == std::string::npos)
		tabs = 0;
	if (space > tabs){
		std::vector<std::string> ret = splitString(line, ' ');
		for (int i = 0; i < ret.size(); i++){
			ret[i].erase(std::remove_if(ret[i].begin(), ret[i].end(), ::isspace), ret[i].end());
			if (ret[i].length() > 0){
				res = ret[i];
				break ;
			}
		}
	}
	else if (tabs > space){
		std::vector<std::string> ret = splitString(line, '	');
		for (int i = 0; i < ret.size(); i++){
			ret[i].erase(std::remove_if(ret[i].begin(), ret[i].end(), ::isspace), ret[i].end());
			if (ret[i].length() > 0){
				res = ret[i];
				break ;
			}
		}
	}
	else{
		res = line;
		res.erase(std::remove_if(res.begin(), res.end(), ::isspace), res.end());
	}
	if (res == "acceptDefault")
		return CORRECT;
	else if (res == "server")
		return CORRECT;
	else if (res == "host")
		return CORRECT;
	else if (res == "server_name")
		return CORRECT;
	else if (res == "listen")
		return CORRECT;
	else if (res == "root")
		return CORRECT;
	else if (res == "index")
		return CORRECT;
	else if (res == "error_page")
		return CORRECT;
	else if (res == "location")
		return CORRECT;
	else if (res == "allowedMethods")
		return CORRECT;
	else if (res == "return")
		return CORRECT;
	else if (res == "client_max_body_size")
		return CORRECT;
	else if (res == "}")
		return CORRECT;
	else if (res.empty())
		return CORRECT;
	writeTimestamp(RED, "\"" + res + "\" ins\'t something valid");
	return -2;
}

bool	parsing::parseConfigFile(){
	int	ret;
	string line;
	unsigned int	nbLine = 0;
	int	serverLineCheck;

	writeTimestamp(PURPLE, "Parsing is starting...");
	if (checkFile() == false){
		return false;
	}
	while (std::getline(configFile, line)){
		if (checkValid(line) == -2)
			return false;
		if (nbLine == 0)
			checkDefault(line);
		if (checkServer(line, nbLine) == -2)
			return (error(SERVERPARS), false);
		checkLocation(line, &nbLine);
		if (checkHost(line, nbLine) == -2)
			return (error(HOST_ERR), false);
		if (checkListen(line, nbLine) == -2)
			return (error(LISTEN_ERR), false);
		if (checkServerName(line, nbLine) == -2)
			return (error(SERVERNAME_ERR), false);
		if (checkRoot(line, nbLine) == -2)
			return (error(ROOT_ERR), false);
		if (checkIndex(line, nbLine) == -2)
			return (error(INDEX_ERR), false);
		if (checkErrorPage(line, nbLine) == -2)
			return (error(ERROR_PAGE_ERR), false);
		if (checkClientMaxBodySize(line, nbLine) == -2)
			return (error(CLIENT_MAX_BODY_SIZE_ERR), false);
		if (checkReturns(line, nbLine) == -2)
			return (error(RETURN_ERR), false);
		nbLine++;
	}
	verifLine.push_back(DONT);
	for (int i = 0; i < verifLine.size(); i++) {
		cout << "line " << i + 1 << " : " << verifLine[i] << endl;
	}
	return true;
}

// client_max_body_size 1B; = 1
// client_max_body_size 1K; = 1000
// client_max_body_size 1M; = 1 000 000
// client_max_body_size 1G; = 1 000 000 000

int	parsing::checkClientMaxBodySize(string const &line, unsigned int nbLine){
	size_t pos = line.find("client_max_body_size");
	if (pos == string::npos)
		return -1;
	if (checkVargule(line, defaultIfError, false, verifLine, nbLine) == false){
		verifLine.push_back(DONT);
		return -2;
	}
	if (checkIdentationParsing("client_max_body_size", 1, line, defaultIfError, "client max body size line", verifLine, nbLine) == false){
		verifLine.push_back(DONT);
		return -2;
	}
	if (isThereSomethingInMyString(line, "client_max_body_size", defaultIfError, false, false, verifLine, nbLine) == true){
		verifLine.push_back(DONT);
		return -2;
	}
	string str = line;
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
	if (str.length() < 22){
		writeTimestamp(YELLOW, "The client_max_body_size line must have another arguments, like an number of bytes (1B, 1K, 1M, 1G)");
		verifLine.push_back(DONT);
		return -2;
	}
	str.erase(0, 20); str.erase(str.size() - 1, str.size());
	if (str.find("B") == string::npos && str.find("K") == string::npos && str.find("M") == string::npos && str.find("G") == string::npos){
		if (defaultIfError == true){
			writeTimestamp(YELLOW, "The arguments must be an number and a \'B\' or a \'K\' or a \'M\' or a \'G\', switching to default");
			if (nbLine == verifLine.size())
				verifLine.push_back(SWITCH);
		}
		else{
			writeTimestamp(RED, "The arguments must be an number and a \'B\' or a \'K\' or a \'M\' or a \'G\'");
			if (nbLine == verifLine.size())
				verifLine.push_back(DONT);
		}
	}
	str.erase(str.size() - 1, str.size());
	cout << str << endl;
	if (containsNonDigit(str) == true){
		if (defaultIfError == true){
			writeTimestamp(YELLOW, "The number of Bytes must only contains number, switching to default");
			if (nbLine == verifLine.size())
				verifLine.push_back(SWITCH);
		}
		else{
			writeTimestamp(RED, "The number of Bytes must only contains number");
			if (nbLine == verifLine.size())
				verifLine.push_back(DONT);
		}
	}
	if (nbLine == verifLine.size())
		verifLine.push_back(OKPARS);
	// cout << "Line : #" << verifLine.size() << " content \"" + line + "\"" << verifLine[verifLine.size() - 1] << endl;
	return CORRECT;
}

int	parsing::checkReturns(string const &line, unsigned int nbLine){
	size_t pos = line.find("return");
	if (pos == string::npos)
		return -1;
	if (checkVargule(line, defaultIfError, false, verifLine, nbLine) == false){
		verifLine.push_back(DONT);
		return -2;
	}
	if (checkIdentationParsing("return", 1, line, defaultIfError, "return line", verifLine, nbLine) == false){
		verifLine.push_back(DONT);
		return -2;
	}
	if (isThereSomethingInMyString(line, "return", defaultIfError, false, true, verifLine, nbLine) == true){
		verifLine.push_back(DONT);
		return -2;
	}
	string str = line;
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
	if (str.length() < 8){
		writeTimestamp(YELLOW, "The return line must have another arguments, like an error code and an html or url!");
		verifLine.push_back(DONT);
		return -2;
	}
	writeTimestamp(GREEN, "Return is OK!");
	if (nbLine == verifLine.size())
		verifLine.push_back(OKPARS);
	// cout << "Line : #" << verifLine.size() << " content \"" + line + "\"" << verifLine[verifLine.size() - 1] << endl;
	return CORRECT;
}

int	parsing::checkErrorPage(string const &line, unsigned int nbLine){
	if (line.find("error_page") == string::npos)
		return -1;
	if (checkVargule(line, defaultIfError, false, verifLine, nbLine) == false){
		verifLine.push_back(DONT);
		return -2;
	}
	if (checkIdentationParsing("error_page", 1, line, defaultIfError, "error_page line", verifLine, nbLine) == false){
		verifLine.push_back(DONT);
		return -2;
	}
	if (isThereSomethingInMyString(line, "error_page", defaultIfError, false, true, verifLine, nbLine) == true){
		verifLine.push_back(DONT);
		return -2;
	}
	string str = line;
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
	if (str.length() < 12){
		if (defaultIfError == false){
			writeTimestamp(RED, "The error_page line must have another arguments, like an error code and html!");
			verifLine.push_back(DONT);
			return -2;
		}
		else
			writeTimestamp(YELLOW, "The error_page line must have another arguments, like an error code and html, switching to default!");
		return -1;
	}
	writeTimestamp(GREEN, "Error_page is OK!");
	if (nbLine == verifLine.size())
		verifLine.push_back(OKPARS);
	// cout << "Line : #" << verifLine.size() << " content \"" + line + "\"" << verifLine[verifLine.size() - 1] << endl;
	return true;
}

int	parsing::checkIndex(string const &line, unsigned int nbLine){
	if (line.find("index") == string::npos || line.find("index") == 2)
		return -1;
	if (checkVargule(line, defaultIfError, false, verifLine, nbLine) == false){
		verifLine.push_back(DONT);
		return -2;
	}
	if (checkIdentationParsing("index", 1, line, defaultIfError, "index line", verifLine, nbLine) == false){
		verifLine.push_back(DONT);
		return -2;
	}
	if (isThereSomethingInMyString(line, "index", defaultIfError, false, false, verifLine, nbLine) == true){
		verifLine.push_back(DONT);
		return -2;
	}
	string str = line;
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
	if (str.length() < 7){
		if (defaultIfError == false){
			writeTimestamp(RED, "The index line must have another arguments, like an html file!");
			verifLine.push_back(DONT);
			return -2;
		}
		else
			writeTimestamp(YELLOW, "The index line must have another arguments, like an html file, switching to default!");
		return -1;
	}
	writeTimestamp(GREEN, "Index is OK!");
	if (nbLine == verifLine.size())
		verifLine.push_back(OKPARS);
	// cout << "Line : #" << verifLine.size() << " content \"" + line + "\"" << verifLine[verifLine.size() - 1] << endl;
	return true;
}

int	parsing::checkRoot(string const &line, unsigned int nbLine){
	if (line.find("root") == string::npos || line.find("root") == 2)
		return -1;
	if (checkVargule(line, defaultIfError, false, verifLine, nbLine) == false){
		verifLine.push_back(DONT);
		return -2;
	}
	if (checkIdentationParsing("root", 1, line, defaultIfError, "root line", verifLine, nbLine) == false){
		verifLine.push_back(DONT);
		return -2;
	}
	if (isThereSomethingInMyString(line, "root", defaultIfError, false, false, verifLine, nbLine) == true){
		verifLine.push_back(DONT);
		return -2;
	}
	string str = line;
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
	if (str.length() < 6){
		if (defaultIfError == false){
			writeTimestamp(RED, "The root line must have another arguments, like a path to the root!");
			verifLine.push_back(DONT);
			return -2;
		}
		else
			writeTimestamp(YELLOW, "The root line must have another arguments, like a path to the root, switching to default!");
		return -1;
	}
	writeTimestamp(GREEN, "Root is OK!");
	if (nbLine == verifLine.size())
		verifLine.push_back(OKPARS);
	// cout << "Line : #" << verifLine.size() << " content \"" + line + "\"" << verifLine[verifLine.size() - 1] << endl;
	return true;
}

int	parsing::checkServerName(string const &line, unsigned int nbLine){
	if (line.find("server_name") == string::npos)
		return -1;
	if (checkVargule(line, defaultIfError, false, verifLine, nbLine) == false){
		verifLine.push_back(DONT);
		return -2;
	}
	if (checkIdentationParsing("server_name", 1, line, defaultIfError, "server_name line", verifLine, nbLine) == false){
		verifLine.push_back(DONT);
		return -2;
	}
	if (isThereSomethingInMyString(line, "server_name", defaultIfError, false, false, verifLine, nbLine) == true){
		verifLine.push_back(DONT);
		return -2;
	}
	string str = line;
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
	if (str.length() < 13){
		if (defaultIfError == false){
			writeTimestamp(RED, "The server_name line must have another arguments, like a name!");
			verifLine.push_back(DONT);
			return -2;
		}
		else
			writeTimestamp(YELLOW, "The server_name line must have another arguments, like a name, switching to default!");
		return -1;
	}
	writeTimestamp(GREEN, "Server_name is OK!");
	if (nbLine == verifLine.size())
		verifLine.push_back(OKPARS);
	// cout << "Line : #" << verifLine.size() << " content \"" + line + "\"" << verifLine[verifLine.size() - 1] << endl;
	return true;
}

int	parsing::checkHost(string const &line, unsigned int nbLine){

	if (line.find("host") == string::npos)
		return -1;
	if (checkVargule(line, defaultIfError, false, verifLine, nbLine) == false){
		verifLine.push_back(DONT);
		return -2;
	}
	if (checkIdentationParsing("host", 1, line, defaultIfError, "host line", verifLine, nbLine) == false){
		verifLine.push_back(DONT);
		return -2;
	}
	if (isThereSomethingInMyString(line, "host", defaultIfError, false, false, verifLine, nbLine) == true){
		verifLine.push_back(DONT);
		return -2;
	}
	string str = line;
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
	if (str.length() < 6){
		if (defaultIfError == false){
			writeTimestamp(RED, "The host line must have another arguments, like an IP!");
			verifLine.push_back(DONT);
			return -2;
		}
		else
			writeTimestamp(YELLOW, "The host line must have another arguments, like an IP, switching to default!");
		return -1;
	}
	writeTimestamp(GREEN, "Host is OK!");
	if (nbLine == verifLine.size())
		verifLine.push_back(OKPARS);
	// cout << "Line : #" << verifLine.size() << " content \"" + line + "\"" << verifLine[verifLine.size() - 1] << endl;
	return true;
}

int	parsing::checkListen(string const &line, unsigned int nbLine){
	if (line.find("listen") == string::npos)
		return -1;
	if (checkVargule(line, defaultIfError, false, verifLine, nbLine) == false){
		verifLine.push_back(DONT);
		return -2;
	}
	if (checkIdentationParsing("listen", 1, line, defaultIfError, "listen line", verifLine, nbLine) == false){
		verifLine.push_back(DONT);
		return -2;
	}
	if (isThereSomethingInMyString(line, "listen", defaultIfError, false, false, verifLine, nbLine) == true){
		verifLine.push_back(DONT);
		return -2;
	}
	if (containsNonDigit(&line[line.find("listen") + 6]) == true){
		if (defaultIfError == false){
			writeTimestamp(RED, "The line \"" + line + "\" must only contain digit in the second arguments");
			verifLine.push_back(DONT);
			return -2;
		}
		else {
			writeTimestamp(YELLOW, "The line \"" + line + "\" must only contain digit in the second arguments, switching to default");
			return -1;
		}
	}
	string str = line;
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
	if (str.length() < 8){
		if (defaultIfError == false){
			writeTimestamp(RED, "The listen line must have another arguments, like a Port!");
			verifLine.push_back(DONT);
			return -2;
		}
		else
			writeTimestamp(YELLOW, "The listen line must have another arguments, like a Port, switching to default!");
		return -1;
	}
	writeTimestamp(GREEN, "Listen is OK!");
	if (nbLine == verifLine.size())
		verifLine.push_back(OKPARS);
	// cout << "Line : #" << verifLine.size() << " content \"" + line + "\"" << verifLine[verifLine.size() - 1] << endl;
	return true;
}

int	parsing::checkServer(std::string const &line, unsigned int nbLine){
	if (line.find("server") == string::npos || line.find("server_name") != string::npos)
		return -1;
	if (line.find("server") > 0){
		writeTimestamp(RED, "The server line mustn\'t have indentation!");
		verifLine.push_back(DONT);
		return -2;
	}
	if (isThereSomethingInMyString(line, "server", false, false, false, verifLine, nbLine) == true){
		verifLine.push_back(DONT);
		return -2;
	}
	else if (line.find("server") != string::npos && line.find("{") == string::npos){
		writeTimestamp(RED, "Error in the server line, you didnt put the \"{\" at the end!");
		verifLine.push_back(DONT);
		return -2;
	}
	if (nbLine == verifLine.size())
		verifLine.push_back(OKPARS);
	// cout << "Line : #" << verifLine.size() << " content \"" + line + "\"" << verifLine[verifLine.size() - 1] << endl;
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
	if (checkVargule(line, defaultIfError, false, verifLine, 0) == false){
		verifLine.push_back(DONT);
		return -2;
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
	if (isThereSomethingInMyString(line, "acceptDefault", false, false, false, verifLine, 0) == true){
		verifLine.push_back(DONT);
		return -1;
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
	verifLine.push_back(DONT);
	// cout << "Line : #" << verifLine.size() << " content \"" + line + "\"" << verifLine[verifLine.size() - 1] << endl;
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
