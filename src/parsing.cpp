#include "../include/parsing.hpp"
#include <fstream>

parsing::parsing(string path): pathConfigFile(path){
	defaultIfError = false;
}

bool	parsing::checkValid(string const &line){
	string	res;
	if (line.find("}") != string::npos && line.length() == 1)
		return true;
	if (line.length() == 0)
		return true;
	if (line.find(" ") == string::npos){
		res = line;
		if (res.find(";") < string::npos)
			res.erase(res.length() - 1, res.length());
	}else{
		std::vector<std::string> ret = splitString(line, ' ');
		for (size_t i = 0; i < ret.size(); i++){
			ret[i].erase(std::remove_if(ret[i].begin(), ret[i].end(), ::isspace), ret[i].end());
			if (ret[i].length() > 0){
				res = ret[i];
				break ;
			}
		}
	}
	res.erase(std::remove_if(res.begin(), res.end(), ::isspace), res.end());
	if (res == "acceptDefault")
		return true;
	else if (res == "server")
		return true;
	else if (res == "host")
		return true;
	else if (res == "server_name")
		return true;
	else if (res == "listen")
		return true;
	else if (res == "root")
		return true;
	else if (res == "index")
		return true;
	else if (res == "error_page")
		return true;
	else if (res == "location")
		return true;
	else if (res == "allowedMethods")
		return true;
	else if (res == "return")
		return true;
	else if (res == "client_max_body_size")
		return true;
	else if (res == "upload_enable")
		return true;
	else if (res == "upload_store")
		return true;
	else if (res == "autoindex")
		return true;
	else if (res == "}")
		return true;
	else if (res == "useCGI")
		return true;
	else if (res.empty())
		return true;
	simpleWriteTimestamp(RED, res + " in the line \"" + line + "\" is not valid");
	return false;
}

bool	parsing::parseConfigFile(){
	string line;
	unsigned int	nbLine = 0;
	if (checkFile() == false){
		return false;
	}
	std::ifstream test;
	test.open(pathConfigFile);
	bool flag = false;
	string linetest;
	while (std::getline(test, linetest)){
		if (findFirstWord(linetest) == "server")
			flag = true;
	}
	if (flag == false){
		simpleWriteTimestamp(RED, "They're no server in the file... ok");
		return false;
	}
	while (std::getline(configFile, line)){
		if (checkValid(line) == false)
			return false;
		if (nbLine == 0)
			checkDefault(line, nbLine);
		if (checkServer(line, &nbLine) == false)
			return false;
		if (findFirstWord(line) == "acceptDefault" && nbLine > 1){
			cout << RED "You can't set acceptDefault again or in another line than the first one" RESET << endl;
		}
		if (nbLine == verifLine.size())
			verifLine.push_back(DONT);
		nbLine++;
	}
	if (nbLine == 1 && defaultIfError == false)
		return false;
	return true;
}

bool	parsing::checkFile(){
	string			line;
	struct stat		fileStat;

	if (stat(pathConfigFile.c_str(), &fileStat) == 0){
		if (S_ISDIR(fileStat.st_mode) == true){
			simpleWriteTimestamp(RED, "The \"File\" isn't one, it's a directory");
			return false;
		}
	}
	else{
		simpleWriteTimestamp(RED, "The ConfigFile you entered doesn't exist");
		return false;
	}
	if (pathConfigFile.find(".cfg") == string::npos){
		simpleWriteTimestamp(RED, "The ConfigFile you entered doesn't have the right extension, the one needed is : \".cfg\"");
		return false;
	}
	configFile.open(pathConfigFile);
	simpleWriteTimestamp(GREEN, "File is ok");
	return true;
}

void	parsing::checkDefault(string const &line, unsigned int nbLine){
	if ((line.find("acceptDefault") > 0 || line.find("acceptDefault") == string::npos)){
		simpleWriteTimestamp(YELLOW, "	You choose that the program will close if they're is an error in the ConfigFile");
		simpleWriteTimestamp(YELLOW, "	If you don't want that, you must include at the beginning of the ConfigFile : \"acceptDefault true\"");
		if (findFirstWord(line) != "server")
			verifLine.push_back(DONT);
		return ;
	}
	if (checkForTabs(line, -1) == false){
		selectMessage(DEFAULT, SPACEERROR, nbLine, "	You choose that the program will close if they're is an error in the ConfigFile. If you don't want that, you must include at the beginning of the ConfigFile : \"acceptDefault true\"");
		verifLine.push_back(DONT);
		return ;
	}
	if (checkVargule(line) == false){
		selectMessage(DEFAULT, VARGULEERR, nbLine, "	You choose that the program will close if they're is an error in the ConfigFile. If you don't want that, you must include at the beginning of the ConfigFile : \"acceptDefault true\"");
		verifLine.push_back(DONT);
		return ;
	}
	if (isThereSomethingInMyString(line, "acceptDefault", line.rfind(" ")) == true){
		selectMessage(DEFAULT, IMPOSTORERROR, nbLine, "	You choose that the program will close if they're is an error in the ConfigFile. If you don't want that, you must include at the beginning of the ConfigFile : \"acceptDefault true\"");
		verifLine.push_back(DONT);
		return ;
	}
	size_t find = line.find("true;");
	if (find != string::npos && find == line.length() - 5){
		defaultIfError = true;
		selectMessage(VALID, NOERR, nbLine, "	The program will switch a line to the default setting if an error occur");
	}else if (line.find("false;") != string::npos && line.find("false;")){
		defaultIfError = false;
		selectMessage(DEFAULT, NOERR, nbLine, "	You choose that the program will close if they're is an error in the ConfigFile. If you don't want that, you must include at the beginning of the ConfigFile : \"acceptDefault true\"");
	}
	verifLine.push_back(DONT);
}

bool	parsing::checkServer(std::string &line, unsigned int *nbLine){
	if (findFirstWord(line) != "server")
		return -1;
	if (line.find("server") > 0){
		selectMessage(WRONG, IDENTATIONERROR, *nbLine, ", it must not have any");
		return false;
	}
	if (checkForTabs(line, -1) == false){
		selectMessage(WRONG, SPACEERROR, *nbLine, "");
		return false;
	}
	if (line.find("{") != line.length() - 1){
		selectMessage(WRONG, MISSINGEND, *nbLine, ", the server line must finish with \"{\"");
		return false;
	}
	if (isThereSomethingInMyString(line, "server", line.length() - 1) == true){
		selectMessage(WRONG, IMPOSTORERROR, *nbLine, "");
		return false;
	}
	verifLine.push_back(OKPARS);
	selectMessage(VALID, NOERR, *nbLine, "	\"" + line + "\"");
	(*nbLine)++;
	while (std::getline(configFile, line)){
		if (checkValid(line) == false)
			return false;
		if (checkLocation(line, nbLine) == false)
			return false;
		if (checkHost(line, *nbLine) == false)
			return false;
		if (checkListen(line, *nbLine) == false)
			return false;
		if (checkServerName(line, *nbLine) == false)
			return false;
		if (checkRoot(line, *nbLine) == false)
			return false;
		if (checkIndex(line, *nbLine) == false)
			return false;
		if (checkErrorPage(line, *nbLine) == false)
			return false;
		if (checkClientMaxBodySize(line, *nbLine) == false)
			return false;
		if (findFirstWord(line) == "server"){
			cout << RED "You tried to make another server inside a server, that's wack, Im not dealing with it..." RESET << endl;
			return false;
		}
		if (findFirstWord(line) == "acceptDefault"){
			wagadooMachine(line, defaultIfError, IMPOSTORERROR, *nbLine, " you can't have this line in the server", defaultIfError, verifLine, false);
			if (defaultIfError == false)
				return false;
		}
		if (findFirstWord(line) == "allowedMethods" == true){
			wagadooMachine(line, defaultIfError, IMPOSTORERROR, *nbLine, " you can't have this line in the server", defaultIfError, verifLine, false);
			if (defaultIfError == false)
				return false;
		}
		if (findFirstWord(line) == "return" == true){
			wagadooMachine(line, defaultIfError, IMPOSTORERROR, *nbLine, " you can't have this line in the server", defaultIfError, verifLine, false);
			if (defaultIfError == false)
				return false;
		}
		if (findFirstWord(line) == "useCGI" == true){
			wagadooMachine(line, defaultIfError, IMPOSTORERROR, *nbLine, " you can't have this line in the server", defaultIfError, verifLine, false);
			if (defaultIfError == false)
				return false;
		}
		if (line.find("}") == 0){
			if (line.find("}") != 0 || line.length() != 1){
				return false;
			}
			verifLine.push_back(DONT);
			selectMessage(VALID, NOERR,*nbLine, " \"" + line + "\"");
			break ;
		}
		if (*nbLine == verifLine.size()){
			cout << YELLOW << "This line: " << line << " doesn't mean anything, skipping it" << endl;
			verifLine.push_back(DONT);
		}
		(*nbLine)++;
	}
	if (line.find("}") != 0){
		simpleWriteTimestamp(RED, "You didnt close the server, quitting the program....");
		return false;
	}
	return true;
}

bool	parsing::checkHost(string const &line, unsigned int nbLine){
	if (findFirstWord(line) != "host")
		return -1;
	if (checkIdentationParsing(line, "host", false) == false){
		wagadooMachine(line, defaultIfError, IDENTATIONERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkVargule(line) == false){
		wagadooMachine(line, defaultIfError, VARGULEERR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkForArgs(line, 6) == false){
		wagadooMachine(line, defaultIfError, NUMBERARGSERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkForTabs(line, 0) == false){
		wagadooMachine(line, defaultIfError, SPACEERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (isThereSomethingInMyString(line, "host", line.rfind(" ")) == true){
		wagadooMachine(line, defaultIfError, IMPOSTORERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	verifLine.push_back(OKPARS);
	selectMessage(VALID, NOERR, nbLine, " \"" + line + "\"");
	return true;
}

bool	parsing::checkListen(string const &line, unsigned int nbLine){
	if (findFirstWord(line) != "listen")
		return -1;
	if (checkIdentationParsing(line, "listen", false) == false){
		wagadooMachine(line, defaultIfError, IDENTATIONERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkVargule(line) == false){
		wagadooMachine(line, defaultIfError, VARGULEERR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkForArgs(line, 8) == false){
		wagadooMachine(line, defaultIfError, NUMBERARGSERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkForTabs(line, 0) == false){
		wagadooMachine(line, defaultIfError, SPACEERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (isThereSomethingInMyString(line, "listen", line.rfind(" ")) == true){
		wagadooMachine(line, defaultIfError, IMPOSTORERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
		return -2;
	}
	if (containsNonDigit(&line[7]) == true){
		wagadooMachine(line, defaultIfError, DIGITERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	try {
		std::stoi(&line[7]);
	} catch (std::exception &e) {
		wagadooMachine(line, defaultIfError, NUMBERTOOBIG, nbLine, ", it must fit in an int", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	verifLine.push_back(OKPARS);
	selectMessage(VALID, NOERR, nbLine, " \"" + line + "\"");
	return true;
}

bool	parsing::checkServerName(string const &line, unsigned int nbLine){
	if (findFirstWord(line) != "server_name")
		return -1;
	if (checkIdentationParsing(line, "server_name", false) == false){
		wagadooMachine(line, defaultIfError, IDENTATIONERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkVargule(line) == false){
		wagadooMachine(line, defaultIfError, VARGULEERR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkForArgs(line, 13) == false){
		wagadooMachine(line, defaultIfError, NUMBERARGSERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkForTabs(line, 0) == false){
		wagadooMachine(line, defaultIfError, SPACEERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (isThereSomethingInMyString(line, "server_name", line.rfind(" ")) == true){
		wagadooMachine(line, defaultIfError, IMPOSTORERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	verifLine.push_back(OKPARS);
	selectMessage(VALID, NOERR, nbLine, " \"" + line + "\"");
	return true;
}

bool	parsing::checkRoot(string const &line, unsigned int nbLine){
	if (findFirstWord(line) != "root")
		return -1;
	if (checkIdentationParsing(line, "root", false) == false){
		wagadooMachine(line, defaultIfError, IDENTATIONERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkVargule(line) == false){
		wagadooMachine(line, defaultIfError, VARGULEERR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkForArgs(line, 6) == false){
		wagadooMachine(line, defaultIfError, NUMBERARGSERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkForTabs(line, 0) == false){
		wagadooMachine(line, defaultIfError, SPACEERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (isThereSomethingInMyString(line, "root", line.rfind(" ")) == true){
		wagadooMachine(line, defaultIfError, IMPOSTORERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	verifLine.push_back(OKPARS);
	selectMessage(VALID, NOERR, nbLine, " \"" + line + "\"");
	return true;
}

bool	parsing::checkIndex(string const &line, unsigned int nbLine){
	if (findFirstWord(line) != "index")
		return -1;
	if (checkIdentationParsing(line, "index", false) == false){
		wagadooMachine(line, defaultIfError, IDENTATIONERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkVargule(line) == false){
		wagadooMachine(line, defaultIfError, VARGULEERR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkForArgs(line, 7) == false){
		wagadooMachine(line, defaultIfError, NUMBERARGSERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkForTabs(line, 0) == false){
		wagadooMachine(line, defaultIfError, SPACEERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (isThereSomethingInMyString(line, "index", line.rfind(" ")) == true){
		wagadooMachine(line, defaultIfError, IMPOSTORERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	verifLine.push_back(OKPARS);
	selectMessage(VALID, NOERR, nbLine, " \"" + line + "\"");
	return true;
}

bool	parsing::checkErrorPage(string const &line, unsigned int nbLine){
	if (findFirstWord(line) != "error_page")
		return -1;
	if (checkIdentationParsing(line, "error_page", false) == false){
		wagadooMachine(line, defaultIfError, IDENTATIONERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkVargule(line) == false){
		wagadooMachine(line, defaultIfError, VARGULEERR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkForArgs(line, 12) == false){
		wagadooMachine(line, defaultIfError, NUMBERARGSERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkForTabs(line, 0) == false){
		wagadooMachine(line, defaultIfError, SPACEERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	std::vector<string> split = splitString(line, ' ');
	for (size_t i = 1; i < split.size() - 1; i++){
		if (i < split.size() - 1 )
			try {
				std::stoi(split[i]);
			} catch (std::exception &e) {
				wagadooMachine(line, defaultIfError, NUMBERTOOBIG, nbLine, ", it must fit in an int", defaultIfError, verifLine, false);
				if (defaultIfError == false)
					return false;
				return true;
			}
		if (containsNonDigit(split[split.size() - 1]) == false){
			wagadooMachine(line, defaultIfError, MISSINGEND, nbLine, ", this line need a HTML to link up with the error code", defaultIfError, verifLine, false);
			if (defaultIfError == false)
				return false;
			return true;
		}
		if (containsNonDigit(split[i]) == true){
			wagadooMachine(line, defaultIfError, DIGITERROR, nbLine, "", defaultIfError, verifLine, false);
			if (defaultIfError == false)
				return false;
			return true;
		}
	}
	verifLine.push_back(OKPARS);
	selectMessage(VALID, NOERR, nbLine, " \"" + line + "\"");
	return true;
}

bool	parsing::checkClientMaxBodySize(string const &line, unsigned int nbLine){
	if (findFirstWord(line) != "client_max_body_size")
		return -1;
	if (checkIdentationParsing(line, "client_max_body_size", false) == false){
		wagadooMachine(line, defaultIfError, IDENTATIONERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkVargule(line) == false){
		wagadooMachine(line, defaultIfError, VARGULEERR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkForArgs(line, 21) == false){
		wagadooMachine(line, defaultIfError, NUMBERARGSERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (checkForTabs(line, 0) == false){
		wagadooMachine(line, defaultIfError, SPACEERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (isThereSomethingInMyString(line, "client_max_body_size", line.rfind(" ")) == true){
		wagadooMachine(line, defaultIfError, IMPOSTORERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	string str = line;
	str.erase(0, 21); str.erase(str.size() - 1, str.size());
	if (str.find("B") == string::npos && str.find("K") == string::npos && str.find("M") == string::npos && str.find("G") == string::npos){
		wagadooMachine(line, defaultIfError, BADARGS, nbLine, "The arguments must be an number and a \'B\' or a \'K\' or a \'M\' or a \'G\', switching to default", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	str.erase(str.size() - 1, str.size());
	try {
		std::stoi(str);
	} catch (std::exception &e) {
		wagadooMachine(line, defaultIfError, NUMBERTOOBIG, nbLine, ", it must fit in an int", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	if (containsNonDigit(str) == true){
		wagadooMachine(line, defaultIfError, DIGITERROR, nbLine, "", defaultIfError, verifLine, false);
		if (defaultIfError == false)
			return false;
		return true;
	}
	verifLine.push_back(OKPARS);
	selectMessage(VALID, NOERR, nbLine, " \"" + line + "\"");
	return true;
}

parsing::~parsing(){
	simpleWriteTimestamp(PURPLE, "Parsing done!");
}
