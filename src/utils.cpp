#include "../include/utils.hpp"
#include <iostream>

std::string	timestamp() {
	std::time_t currentTime = std::time(nullptr);
	std::tm* localTime = std::localtime(&currentTime);
	char timestampValue[21];
	std::strftime(timestampValue, sizeof(timestampValue), "[%Y-%m-%d %H:%M:%S]", localTime);
	std::string tmp = timestampValue;
	return (tmp);
}

void writeTimestamp(std::string color, std::string const &message){
	std::cout << color << timestamp() << " " << message << RESET << std::endl;
}

bool	checkIdentationParsing(std::string const &firstWord, int nbIndentation, std::string const &line, bool inLocation, bool defaultIfError, std::string const &lineName){
	if (line.find(firstWord) != nbIndentation){
		if(defaultIfError == true){
				writeTimestamp(YELLOW, "Error of identation in " + lineName + ", switching to default setting");
				return true;
		}
		else if(defaultIfError == false){
			writeTimestamp(RED, "Error of identation in " + lineName + ", must have one, default setting is off, exiting program...");
			return false;
		}
	}
	if (inLocation == false){
		if (line.find("	") != 0){
			if(defaultIfError == true){
				writeTimestamp(YELLOW, "Error of identation in " + lineName + ", must have one, switching to default setting");
				return true;
			}
			else if(defaultIfError == false){
				writeTimestamp(RED, "Error of identation in " + lineName + ", must have one, default setting is off, exiting program...");
				return false;
			}
		}
	}
	else if (inLocation == true){
		if (line.find("		") != 0){
			if(defaultIfError == true){
				writeTimestamp(YELLOW, "Eror of identation in " + lineName + ", must have one, switching to default setting");
				return true;
			}
			else if(defaultIfError == false){
				writeTimestamp(RED, "Eror of identation in " + lineName + ", must have one, default setting is off, exiting program...");
				return false;
			}
		}
	}
	return true;
}


bool isThereSomethingInMyString(std::string const &input, std::string const &beginning, std::string const& end, bool write){

	size_t start;
	size_t finish;
	std::string impostor;

	start = input.find(beginning);
	finish = input.find(end);
	impostor = input.substr(start + beginning.length(), (finish - (start + beginning.length())));
	impostor.erase(std::remove_if(impostor.begin(), impostor.end(), ::isspace), impostor.end());
	if (impostor.length() > 0){
		if (write == false){
			std::cout << RED << timestamp() << " The \"" << impostor << "\" inside de string \"" << input << "\" is not accepted!" << RESET << std::endl;
		}
		return true;
	}
	return false;
}