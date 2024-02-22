#include "../include/parsing.hpp"
#include <string>

int parsing::isThisTheEnd(string const &line, bool insideLocation){
	size_t pos = line.find("}");
	if (pos == string::npos){
		return false;
	}
	if (insideLocation == true){
		if (pos != 1)
			return false;
	}else{
		if (pos != 0)
			return false;
	}
	// if (pos != line.length() - 1)
		// writeTimestamp(YELLOW, "The line must end with \"}\", this location will not be used");
	return true;
}

bool	parsing::checkLocation(string &line, unsigned int *nbLine){
	if (line.find("location") != 1)
		return -1;
	cout << BLUE << "[" << RESET << std::to_string(*nbLine + 1) << BLUE << "]";simpleWriteTimestamp(BLUE, "Checking new location");
	if (isThereSomethingInMyString(line, "location", line.rfind(" ")) == true){
		wagadooMachine(line, defaultIfError, IMPOSTORERROR, *nbLine, "", defaultIfError, verifLine, true);
		while (isThisTheEnd(line, true) != true){
			(*nbLine)++;
			cout << RED << "[" << RESET << std::to_string(*nbLine + 1) << RED << "]";simpleWriteTimestamp(RED, "This line \"" + line + "\" will not be used because the location line was wrong, all the location element will not be used");
			std::getline(configFile, line);
			verifLine.push_back(DONT);
		}
		return true;
	}
	verifLine.push_back(OKPARS);
	while (isThisTheEnd(line, true) != true){
		if (checkValid(line) == false){
			verifLine.push_back(DONT);
		}
		checkIndexLocation(line, *nbLine);
		checkRootLocation(line, *nbLine);
		checkAllowedMethods(line, *nbLine);
		checkErrorPageLocation(line, *nbLine);
		checkReturnsLocation(line, *nbLine);
		(*nbLine)++;
		std::getline(configFile, line);
	}
	verifLine.push_back(DONT);
	// cout << "IN LOC Line : #" << verifLine.size() << " content \"" + line + "\"" << verifLine[verifLine.size() - 1] << endl;
	return true;
}

void	parsing::checkIndexLocation(string const &line, unsigned int nbLine){
	if (line.find("index") != 2)
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
	if (nbLine == verifLine.size())
		verifLine.push_back(OKPARS);
	selectMessage(VALID, NOERR, nbLine, "	\"" + line + "\"");
	if (nbLine == verifLine.size())
		verifLine.push_back(OKPARS);
}

void	parsing::checkRootLocation(string const &line, unsigned int nbLine){
	if (line.find("root") != 2)
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
	if (nbLine == verifLine.size())
		verifLine.push_back(OKPARS);
	selectMessage(VALID, NOERR, nbLine, "	\"" + line + "\"");
	if (nbLine == verifLine.size())
		verifLine.push_back(OKPARS);
}

void	parsing::checkAllowedMethods(string const &line, unsigned int nbLine){
	if (line.find("allowedMethods") != 2)
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
	selectMessage(VALID, NOERR, nbLine, "	\"" + line + "\"");
	if (nbLine == verifLine.size())
		verifLine.push_back(OKPARS);
}

void	parsing::checkErrorPageLocation(string const &line, unsigned int nbLine){
	if (line.find("error_page") != 2)
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
	if (nbLine == verifLine.size())
		verifLine.push_back(OKPARS);
	selectMessage(VALID, NOERR, nbLine, "	\"" + line + "\"");
	if (nbLine == verifLine.size())
		verifLine.push_back(OKPARS);
}

void	parsing::checkReturnsLocation(string const &line, unsigned int nbLine){
	if (line.find("return") != 2)
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
	if (split[split.size() - 1].substr(0, 5) == "https"){
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
	if (nbLine == verifLine.size())
		verifLine.push_back(OKPARS);
	selectMessage(VALID, NOERR, nbLine, "	\"" + line + "\"");
	if (nbLine == verifLine.size())
		verifLine.push_back(OKPARS);
}