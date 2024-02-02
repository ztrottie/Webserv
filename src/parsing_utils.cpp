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
	if (line.find(";") != line.length() - 1){
	writeTimestamp(RED, "You need to have a \";\" at the end of the index line, this line will not be");
	return -2;
	}
	if (pos != 2){
		writeTimestamp(YELLOW, "Inside the location scope, the \"" + line + "\" must have 2 tabs before the line, we will not use this line...");
		return -2;
	}
	if (line.rfind(" ") > line.rfind("	")){
		if (isThereSomethingInMyString(line, "index", &line[line.rfind(" ")], defaultIfError) == true && defaultIfError)
			return -2;
	}
	else{
		if (isThereSomethingInMyString(line, "index", &line[line.rfind("	")], defaultIfError) == true && defaultIfError)
			return -2;
	}
	writeTimestamp(GREEN, "Index is ok!");
	return CORRECT;
}

int	parsing::checkRootLocation(string const &line){
	size_t pos = line.find("root");
	if (pos == string::npos)
		return -1;
	if (line.find(";") != line.length() - 1){
	writeTimestamp(RED, "You need to have a \";\" at the end of the root line, this line will not be");
	return -2;
	}
	if (pos != 2){
		writeTimestamp(YELLOW, "Inside the location scope, the \"" + line + "\" must have 2 tabs before the line, we will not use this line...");
		return -2;
	}
	if (line.rfind(" ") > line.rfind("	")){
		if (isThereSomethingInMyString(line, "root", &line[line.rfind(" ")], defaultIfError) == true && defaultIfError)
			return -2;
	}
	else{
		if (isThereSomethingInMyString(line, "root", &line[line.rfind("	")], defaultIfError) == true && defaultIfError)
			return -2;
	}
	writeTimestamp(GREEN, "Root is ok!");
	return CORRECT;
}

int	parsing::checkAllowedMethods(string const &line){
	size_t pos = line.find("allowedMethods");
	if (pos == string::npos)
		return -1;
	if (line.find(";") != line.length() - 1){
	writeTimestamp(RED, "You need to have a \";\" at the end of the allowedMethods line, this line will not be");
	return -2;
	}
	if (pos != 2){
		writeTimestamp(YELLOW, "Inside the location scope, the \"" + line + "\" must have 2 tabs before the line, we will not use this line...");
		return -2;
	}
	writeTimestamp(GREEN, "AllowedMethods is ok!");
	return CORRECT;
}

int	parsing::checkErrorPageLocation(string const &line){
	size_t pos = line.find("error_page");
	if (pos == string::npos)
		return -1;
	if (line.find(";") != line.length() - 1){
	writeTimestamp(RED, "You need to have a \";\" at the end of the error_page line, this line will not be");
	return -2;
	}
	if (pos != 2){
		writeTimestamp(YELLOW, "Inside the location scope, the \"" + line + "\" must have 2 tabs before the line, we will not use this line...");
		return -2;
	}
	writeTimestamp(GREEN, "Error_page is ok!");
	return CORRECT;
}

int	parsing::checkReturnsLocation(string const &line){
	size_t pos = line.find("return");
	if (pos == string::npos)
		return -1;
	if (line.find(";") != line.length() - 1){
	writeTimestamp(RED, "You need to have a \";\" at the end of the return line, this line will not be");
	return -2;
	}
	if (pos != 2){
		writeTimestamp(YELLOW, "Inside the location scope, the \"" + line + "\" must have 2 tabs before the line, we will not use this line...");
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