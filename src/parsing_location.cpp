#include "../include/parsing.hpp"
#include <algorithm>
#include <string>
#include <unistd.h>

int parsing::isThisTheEnd(string const &line, bool insideLocation){
	size_t pos = line.find("}");
	if (pos == string::npos){
		return false;
	}
	if (insideLocation == true){
		if (pos == 1)
			return true;
	}else{
		if (pos == 0){
			return true;
		}
	}
	return false;
}

bool	parsing::checkLocation(string &line, unsigned int *nbLine){
	if (findFirstWord(line) != "location")
		return -1;
	cout << BLUE << "[" << RESET << std::to_string(*nbLine + 1) << BLUE << "]"; simpleWriteTimestamp(BLUE, "Checking new location");
	string name = line.substr(line.rfind(" ") + 1, line.rfind("{") - (line.rfind(" ") + 1));
	if (isThereSomethingInMyString(line, "location", line.rfind(" ")) == true || line.rfind("{") != line.length() - 1 ||
		name.empty() || name[0] != '/'){
		wagadooMachine(line, defaultIfError, IMPOSTORERROR, *nbLine, "", defaultIfError, verifLine, true);
		if (line.rfind("{") != line.length() - 1)
			return true;
		while (isThisTheEnd(line, true) != true && !line.empty() && isThisTheEnd(line, false) != true){
			cout << YELLOW << "[" << RESET << std::to_string(*nbLine + 1) << YELLOW << "]";simpleWriteTimestamp(YELLOW, "This line \"" + line + "\" will not be used because the location line was wrong, all the location element will not be used");
			(*nbLine)++;
			std::getline(configFile, line);
			verifLine.push_back(DONT);
		}
		return true;
	}
	verifLine.push_back(OKPARS);
	selectMessage(VALID, NOERR, *nbLine, " \"" + line + "\"");
	(*nbLine)++;
	while (std::getline(configFile, line)){
		if (findFirstWord(line) == "location"){
			simpleWriteTimestamp(RED, "You can't have a location inside another one, exiting program");
			return false;
		}
		else if (findFirstWord(line) == "server"){
			simpleWriteTimestamp(RED, "This line can't be inside the location : " + line);
			verifLine.push_back(DONT);
		}
		else if (findFirstWord(line) == "server_name"){
			simpleWriteTimestamp(RED, "This line can't be inside the location : " + line);
			verifLine.push_back(DONT);
		}
		else if (findFirstWord(line) == "listen"){
			simpleWriteTimestamp(RED, "This line can't be inside the location : " + line);
			verifLine.push_back(DONT);
		}
		else if (findFirstWord(line) == "host"){
			simpleWriteTimestamp(RED, "This line can't be inside the location : " + line);
			verifLine.push_back(DONT);
		}
		else if (line.find("}") < string::npos){
			if (line.find("}") != 1 || line.length() != 2){
				simpleWriteTimestamp(RED, "Location wasn't correctly closed, exiting program");
				return false;
			}
			verifLine.push_back(DONT);
			break;
		}
		else if (checkValid(line) == false){
			verifLine.push_back(DONT);
		}else{
			checkIndexLocation(line, *nbLine);
			checkRootLocation(line, *nbLine);
			checkAllowedMethods(line, *nbLine);
			checkErrorPageLocation(line, *nbLine);
			checkReturnsLocation(line, *nbLine);
			checkClientMaxBodySizeLocation(line, *nbLine);
			checkUploadEnable(line, *nbLine);
			checkUploadStore(line, *nbLine);
			checkAutoIndex(line, *nbLine);
		}
		(*nbLine)++;
	}
	selectMessage(VALID, NOERR, *nbLine, " \"" + line + "\"");
	return true;
}

void	parsing::checkUploadEnable(string const &line, unsigned int nbLine){
	if (findFirstWord(line) != "upload_enable")
		return ;
	if (checkIdentationParsing(line, "upload_enable", true) == false){
		wagadooMachine(line, defaultIfError, IDENTATIONERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkVargule(line) == false){
		wagadooMachine(line, defaultIfError, VARGULEERR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkForArgs(line, 15) == false){
		wagadooMachine(line, defaultIfError, NUMBERARGSERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkForTabs(line, 1) == false){
		wagadooMachine(line, defaultIfError, SPACEERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (isThereSomethingInMyString(line, "upload_enable", line.rfind(" ")) == true){
		wagadooMachine(line, defaultIfError, IMPOSTORERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	string temp = line.substr(line.rfind(" ") + 1, line.length());
	if (temp != "true;" && temp != "false;"){
		wagadooMachine(line, defaultIfError, MISSINGEND, nbLine, ", the last arguments must be true or false", defaultIfError, verifLine, true);
		return ;
	}
	selectMessage(VALID, NOERR, nbLine, " \"" + line + "\"");
	verifLine.push_back(OKPARS);
}

void	parsing::checkUploadStore(string const &line, unsigned int nbLine){
	if (findFirstWord(line) != "upload_store")
		return ;
	if (checkIdentationParsing(line, "upload_store", true) == false){
		wagadooMachine(line, defaultIfError, IDENTATIONERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkVargule(line) == false){
		wagadooMachine(line, defaultIfError, VARGULEERR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkForArgs(line, 14) == false){
		wagadooMachine(line, defaultIfError, NUMBERARGSERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkForTabs(line, 1) == false){
		wagadooMachine(line, defaultIfError, SPACEERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (isThereSomethingInMyString(line, "upload_store", line.rfind(" ")) == true){
		wagadooMachine(line, defaultIfError, IMPOSTORERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	selectMessage(VALID, NOERR, nbLine, " \"" + line + "\"");
	verifLine.push_back(OKPARS);
}

void	parsing::checkIndexLocation(string const &line, unsigned int nbLine){
	if (findFirstWord(line) != "index")
		return ;
	if (checkIdentationParsing(line, "index", true) == false){
		wagadooMachine(line, defaultIfError, IDENTATIONERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkVargule(line) == false){
		wagadooMachine(line, defaultIfError, VARGULEERR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkForArgs(line, 7) == false){
		wagadooMachine(line, defaultIfError, NUMBERARGSERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkForTabs(line, 1) == false){
		wagadooMachine(line, defaultIfError, SPACEERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (isThereSomethingInMyString(line, "index", line.rfind(" ")) == true){
		wagadooMachine(line, defaultIfError, IMPOSTORERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	selectMessage(VALID, NOERR, nbLine, " \"" + line + "\"");
	verifLine.push_back(OKPARS);
}

void	parsing::checkRootLocation(string const &line, unsigned int nbLine){
	if (findFirstWord(line) != "root")
		return ;
	if (checkIdentationParsing(line, "root", true) == false){
		wagadooMachine(line, defaultIfError, IDENTATIONERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkVargule(line) == false){
		wagadooMachine(line, defaultIfError, VARGULEERR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkForArgs(line, 6) == false){
		wagadooMachine(line, defaultIfError, NUMBERARGSERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkForTabs(line, 1) == false){
		wagadooMachine(line, defaultIfError, SPACEERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (isThereSomethingInMyString(line, "root", line.rfind(" ")) == true){
		wagadooMachine(line, defaultIfError, IMPOSTORERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	selectMessage(VALID, NOERR, nbLine, " \"" + line + "\"");
	verifLine.push_back(OKPARS);
}

void	parsing::checkAllowedMethods(string &line, unsigned int nbLine){
	if (findFirstWord(line) != "allowedMethods")
		return ;
	if (checkIdentationParsing(line, "allowedMethods", true) == false){
		wagadooMachine(line, defaultIfError, IDENTATIONERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkVargule(line) == false){
		wagadooMachine(line, defaultIfError, VARGULEERR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkForArgs(line, 16) == false){
		wagadooMachine(line, defaultIfError, NUMBERARGSERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkForTabs(line, 1) == false){
		wagadooMachine(line, defaultIfError, SPACEERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (verifyAllowedMethods(line) == false){
		wagadooMachine(line, defaultIfError, WRONGMETHODS, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	selectMessage(VALID, NOERR, nbLine, " \"" + line + "\"");
	verifLine.push_back(OKPARS);
}

void	parsing::checkErrorPageLocation(string const &line, unsigned int nbLine){
	if (findFirstWord(line) != "error_page")
		return ;
	if (checkIdentationParsing(line, "error_page", true) == false){
		wagadooMachine(line, defaultIfError, IDENTATIONERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkVargule(line) == false){
		wagadooMachine(line, defaultIfError, VARGULEERR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkForArgs(line, 12) == false){
		wagadooMachine(line, defaultIfError, NUMBERARGSERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkForTabs(line, 1) == false){
		wagadooMachine(line, defaultIfError, SPACEERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	std::vector<string> split = splitString(line, ' ');
	for (size_t i = 1; i < split.size() - 1; i++){
		if (containsNonDigit(split[split.size() - 1]) == false){
			wagadooMachine(line, defaultIfError, MISSINGEND, nbLine, ", this line need a HTML to link up with the error code", defaultIfError, verifLine, true);
			return ;
		}
		if (containsNonDigit(split[i]) == true){
			wagadooMachine(line, defaultIfError, DIGITERROR, nbLine, "", defaultIfError, verifLine, true);
			return ;
		}
	}
	selectMessage(VALID, NOERR, nbLine, " \"" + line + "\"");
	verifLine.push_back(OKPARS);
}

void	parsing::checkReturnsLocation(string const &line, unsigned int nbLine){
	if (findFirstWord(line) != "return")
		return ;
	if (checkIdentationParsing(line, "return", true) == false){
		wagadooMachine(line, defaultIfError, IDENTATIONERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkVargule(line) == false){
		wagadooMachine(line, defaultIfError, VARGULEERR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkForArgs(line, 8) == false){
		wagadooMachine(line, defaultIfError, NUMBERARGSERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkForTabs(line, 1) == false){
		wagadooMachine(line, defaultIfError, SPACEERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	std::vector<string> split = splitString(line, ' ');
	if (split[split.size() - 1].length() > 5 && split[split.size() - 1].substr(split[split.size() - 1].length() - 6, split[split.size() - 1].length()) == ".html;"){
		;
	}else if (split[split.size() - 1].substr(0, 5) == "https"){
		;
	}else if (split[split.size() - 1][0] == '/' || split[split.size() - 1][0] == '"' || split[split.size() - 1][0] == '$'){
		;
	}else if(containsNonDigit(split[split.size() - 1]) == false){
		;
	}
	else{
		wagadooMachine(line, defaultIfError, WRONGRETURN, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	for (size_t i = 1; i <= split.size() - 2; i++){
		if (containsNonDigit(split[i]) == true){
			wagadooMachine(line, defaultIfError, DIGITERROR, nbLine, "", defaultIfError, verifLine, true);
			return ;
		}
	}
	selectMessage(VALID, NOERR, nbLine, " \"" + line + "\"");
	verifLine.push_back(OKPARS);
}

void	parsing::checkClientMaxBodySizeLocation(string const &line, unsigned int nbLine){
	if (findFirstWord(line) != "client_max_body_size")
		return ;
	if (checkIdentationParsing(line, "client_max_body_size", true) == false){
		wagadooMachine(line, defaultIfError, IDENTATIONERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkVargule(line) == false){
		wagadooMachine(line, defaultIfError, VARGULEERR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkForArgs(line, 21) == false){
		wagadooMachine(line, defaultIfError, NUMBERARGSERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkForTabs(line, 1) == false){
		wagadooMachine(line, defaultIfError, SPACEERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (isThereSomethingInMyString(line, "client_max_body_size", line.rfind(" ")) == true){
		wagadooMachine(line, defaultIfError, IMPOSTORERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	string str = line;
	str.erase(0, 22); str.erase(str.size() - 1, str.size());
	if (str.find("B") == string::npos && str.find("K") == string::npos && str.find("M") == string::npos && str.find("G") == string::npos){
		wagadooMachine(line, defaultIfError, BADARGS, nbLine, "The arguments must be an number and a \'B\' or a \'K\' or a \'M\' or a \'G\', switching to default", defaultIfError, verifLine, true);
		return ;
	}
	str.erase(str.size() - 1, str.size());
	if (containsNonDigit(str) == true){
		wagadooMachine(line, defaultIfError, DIGITERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	verifLine.push_back(OKPARS);
	selectMessage(VALID, NOERR, nbLine, " \"" + line + "\"");
}

void	parsing::checkAutoIndex(string const &line, unsigned int nbLine){
	if (findFirstWord(line) != "autoindex")
		return ;
	if (checkIdentationParsing(line, "autoindex", true) == false){
		wagadooMachine(line, defaultIfError, IDENTATIONERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkVargule(line) == false){
		wagadooMachine(line, defaultIfError, VARGULEERR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkForArgs(line, 11) == false){
		wagadooMachine(line, defaultIfError, NUMBERARGSERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (checkForTabs(line, 1) == false){
		wagadooMachine(line, defaultIfError, SPACEERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	if (isThereSomethingInMyString(line, "autoindex", line.rfind(" ")) == true){
		wagadooMachine(line, defaultIfError, IMPOSTORERROR, nbLine, "", defaultIfError, verifLine, true);
		return ;
	}
	string temp = line.substr(line.rfind(" ") + 1, line.length());
	if (temp != "true;" && temp != "false;"){
		wagadooMachine(line, defaultIfError, MISSINGEND, nbLine, ", the last arguments must be true or false", defaultIfError, verifLine, true);
		return ;
	}
	selectMessage(VALID, NOERR, nbLine, " \"" + line + "\"");
	verifLine.push_back(OKPARS);
}