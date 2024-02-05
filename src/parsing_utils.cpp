#include "../include/parsing.hpp"

int parsing::isThisTheEnd(string const &line){
	size_t pos = line.find("}");
	if (pos == string::npos){
		return false;
	}
	if (pos != line.length() - 1)
		writeTimestamp(YELLOW, "The line must end with \"}\", this location will not be used");
	return true;
}

int	parsing::checkIndexLocation(string const &line){
	size_t pos = line.find("index");
	if (pos == string::npos)
		return -1;
	if (checkVargule(line, defaultIfError, true) == false)
		return -2;
	if (pos != 2){
		writeTimestamp(YELLOW, "Inside the location scope, the \"" + line + "\" must have 2 tabs before the line, we will not use this line...");
		return -2;
	}
	if (isThereSomethingInMyString(line, "index", defaultIfError, true, false) == true)
		return -2;
	string str = line;
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
	if (str.length() < 7){
		writeTimestamp(YELLOW, "The index line must have another arguments, like an HTML file!");
		return -2;
	}
	writeTimestamp(GREEN, "Index is ok!");
	return CORRECT;
}

int	parsing::checkRootLocation(string const &line){
	size_t pos = line.find("root");
	if (pos == string::npos)
		return -1;
	if (checkVargule(line, defaultIfError, true) == false)
		return -2;
	if (pos != 2){
		writeTimestamp(YELLOW, "Inside the location scope, the \"" + line + "\" must have 2 tabs before the line, we will not use this line...");
		return -2;
	}
	if (isThereSomethingInMyString(line, "root", defaultIfError, true, false) == true)
		return -2;
	string str = line;
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
	if (str.length() < 5){
		writeTimestamp(YELLOW, "The root line must have another arguments, like a path to the server root!");
		return -2;
	}
	writeTimestamp(GREEN, "Root is ok!");
	return CORRECT;
}

int	parsing::checkAllowedMethods(string const &line){
	size_t pos = line.find("allowedMethods");
	if (pos == string::npos)
		return -1;
	if (checkVargule(line, defaultIfError, true) == false)
		return -2;
	if (pos != 2){
		writeTimestamp(YELLOW, "Inside the location scope, the \"" + line + "\" must have 2 tabs before the line, we will not use this line...");
		return -2;
	}
	string str = line;
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
	if (str.length() < 16){
		writeTimestamp(YELLOW, "The allowedMethods line must have another arguments, like (GET, PUT, POST, PATCH, DELETE, CONNECT, OPTIONS, TRACE)!");
		return -2;
	}
	verifyAllowedMethods(line);
	writeTimestamp(GREEN, "AllowedMethods is ok!");
	return CORRECT;
}

int	parsing::checkErrorPageLocation(string const &line){
	size_t pos = line.find("error_page");
	if (pos == string::npos)
		return -1;
	if (checkVargule(line, defaultIfError, true) == false)
		return -2;
	if (pos != 2){
		writeTimestamp(YELLOW, "Inside the location scope, the \"" + line + "\" must have 2 tabs before the line, we will not use this line...");
		return -2;
	}
	if (isThereSomethingInMyString(line, "error_page", defaultIfError, true, true) == true)
		return -2;
	string str = line;
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
	if (str.length() < 12){
		writeTimestamp(YELLOW, "The error_page line must have another arguments, like an error code and html!");
		return -2;
	}
	writeTimestamp(GREEN, "Error_page is ok!");
	return CORRECT;
}

int	parsing::checkReturnsLocation(string const &line){
	size_t pos = line.find("return");
	if (pos == string::npos)
		return -1;
	if (checkVargule(line, defaultIfError, true) == false)
		return -2;
	if (pos != 2){
		writeTimestamp(YELLOW, "Inside the location scope, the \"" + line + "\" must have 2 tabs before the line, we will not use this line...");
		return -2;
	}
	if (isThereSomethingInMyString(line, "return", defaultIfError, true, true) == true)
		return -2;
	string str = line;
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
	if (str.length() < 8){
		writeTimestamp(YELLOW, "The return line must have another arguments, like an error code and an html or url!");
		return -2;
	}
	writeTimestamp(GREEN, "Return is ok!");
	return CORRECT;
}

int	parsing::checkLocation(string &line){
	if (line.find("location") == string::npos)
		return -1;
	writeTimestamp(BLUE, "Checking new location");
	string ret = retIsThereSomethingInMyString(line, "location", "/", true);
	if (ret.length() != 0){
		writeTimestamp(YELLOW, "The \"" + ret + "\" that is in : \"" + line + "\" is not accepted, so this location will not be used");
		return -1;
	}
	while (isThisTheEnd(line) != true){
		if (checkIndexLocation(line) == -2){

		}
		if (checkRootLocation(line) == -2){

		}
		if (checkAllowedMethods(line) == -2){

		}
		if (checkErrorPageLocation(line) == -2){

		}
		if (checkReturnsLocation(line) == -2){

		}
		std::getline(configFile, line);
	}
	writeTimestamp(BLUE, "The Location is ok!");
	return true;
}