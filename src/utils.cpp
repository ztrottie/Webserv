#include "../include/utils.hpp"
#include <vector>

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

bool	checkIdentationParsing(std::string const &firstWord, int nbIndentation, std::string const &line, bool defaultIfError, std::string const &lineName){
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
	return true;
}

bool containsNonDigit(const std::string& str){
	for (size_t i = 0; i < str.length(); ++i){
		if (!isdigit(str[i]) && str[i] != ';' && str[i] != ' ' && str[i] != '	' && str[i] != '\n'){
			return true;
		}
	}
	return false;
}

bool isThereSomethingInMyString(std::string const &line, std::string const &begin, bool _defaultIfError, bool inLocation, bool checkNumber){
	std::string impasta;
	
	size_t space = line.rfind(" ");
	if (space == std::string::npos)
		space = 0;
	size_t tabs = line.rfind("	");
	if (tabs == std::string::npos)
		tabs = 0;
	if (space > tabs){
		impasta = retIsThereSomethingInMyString(line, begin, &line[line.rfind(" ")], !checkNumber);
		if (inLocation == false){
			if (checkNumber == true){
				if (containsNonDigit(impasta) == true){
					if (_defaultIfError == false){
						writeTimestamp(RED, "The \"" + impasta + "\" in the line \"" + line + "\" must not contain any letter, quitting the program");
						return true;
					}
					else {
						writeTimestamp(YELLOW, "The \"" + impasta + "\" in the line \"" + line + "\" must not contain any letter, switching to default setting");
						return false;
					}
				}
			}
			else {
				if (impasta.length() > 0){
					if (_defaultIfError == false){
						writeTimestamp(RED, "The \"" + impasta + "\" in the line \"" + line + "\" isn\'t accepted, quitting the program...");
						return true;
					}
					else {
						writeTimestamp(YELLOW, "The \"" + impasta + "\" in the line \"" + line + "\" isn\'t accepted, switching to default setting");
						return false;
					}
				}
			}
		}
		else {
			if (checkNumber == true){
				if (containsNonDigit(impasta) == true){
					writeTimestamp(YELLOW, "The \"" + impasta + "\" in the line \"" + line + "\" must not contain any letter, this line will not be used");
					return false;
				}
			}
			else {
				if (impasta.length() > 0){
					writeTimestamp(YELLOW, "The \"" + impasta + "\" in the line \"" + line + "\" isn\'t accepted, this line will not be used");
					return false;
				}
			}
		}
	}
	else if (space < tabs){
		impasta = retIsThereSomethingInMyString(line, begin, &line[line.rfind("	")], !checkNumber);
		if (inLocation == false){
			if (checkNumber == true){
				if (containsNonDigit(impasta) == true){
					if (_defaultIfError == false){
						writeTimestamp(RED, "The \"" + impasta + "\" in the line \"" + line + "\" must not contain any letter, quitting the program");
						return true;
					}
					else {
						writeTimestamp(YELLOW, "The \"" + impasta + "\" in the line \"" + line + "\" must not contain any letter, switching to default setting");
						return false;
					}
				}
			}
			else {
				if (impasta.length() > 0){
					if (_defaultIfError == false){
						writeTimestamp(RED, "The \"" + impasta + "\" in the line \"" + line + "\" isn\'t accepted, quitting the program...");
						return true;
					}
					else {
						writeTimestamp(YELLOW, "The \"" + impasta + "\" in the line \"" + line + "\" isn\'t accepted, switching to default setting");
						return false;
					}
				}
			}
		}
		else {
			if (checkNumber == true){
				if (containsNonDigit(impasta) == true){
					writeTimestamp(YELLOW, "The \"" + impasta + "\" in the line \"" + line + "\" must not contain any letter, this line will not be used");
					return false;
				}
			}
			else {
				if (impasta.length() > 0){
					writeTimestamp(YELLOW, "The \"" + impasta + "\" in the line \"" + line + "\" isn\'t accepted, this line will not be used");
					return false;
				}
			}
		}
	}
	return false;
}

std::string retIsThereSomethingInMyString(std::string const &input, std::string const &beginning, std::string const& end, bool erase){
	size_t start;
	size_t finish;
	std::string impostor;

	start = input.find(beginning);
	finish = input.find(end);
	impostor = input.substr(start + beginning.length(), (finish - (start + beginning.length())));
	if (erase == true)
		impostor.erase(std::remove_if(impostor.begin(), impostor.end(), ::isspace), impostor.end());
	if (impostor.length() > 0){
		return impostor;
	}
	return impostor;
}

std::vector<std::string> splitString(std::string const &input, char delimiter){
	std::vector<std::string> result;
	size_t start = 0;
	size_t end = input.find(delimiter);

	while (end != std::string::npos) {
		result.push_back(input.substr(start, end - start));
		start = end + 1;
		end = input.find(delimiter, start);
	}
	result.push_back(input.substr(start));
	return result;
}

bool	verifyAllowedMethods(std::string const &line){
	std::vector<std::string> res = splitString(line, ',');
	for (int i = 0 ; i < res.size(); i++){
		if (i == 0)
			res[i].erase(0, 17);
		else if (i < res.size() - 1)
			res[i].erase(0, 1);
		else if (i < res.size()){
			res[i].erase(0, 1);
			res[i].erase(res[i].size() - 1, res[i].size());
		}
	}
	for (int i = 0 ; i < res.size(); i++){
		if (res[i] == "GET")
			continue;
		else if (res[i] == "PUT")
			continue;
		else if (res[i] == "POST")
			continue;
		else if (res[i] == "PATCH")
			continue;
		else if (res[i] == "DELETE")
			continue;
		else if (res[i] == "CONNECT")
			continue;
		else if (res[i] == "OPTIONS")
			continue;
		else if (res[i] == "TRACE")
			continue;
		else{
			writeTimestamp(YELLOW, "\"" + res[i] + "\" is not a valid method, this line \"" + line + "\" will not be used");
			return false;
		}
	}
	return true;
}

bool	checkVargule(std::string const &line, bool _defaultIfError, bool insideLocation){
	if (line.find(";") != line.length() - 1){
		if (insideLocation == false){
			if (_defaultIfError == false){
				writeTimestamp(RED, "You need to have a \";\" at the end of the line \"" + line + "\"");
				return false;
			}
			else{
				writeTimestamp(YELLOW, "You need to have a \";\" at the end of the line \"" + line + "\" , switching to default!");
				return true;
			}
		}
		else {
			writeTimestamp(YELLOW, "You need to have a \";\" at the end of the line \"" + line + "\" , this line will not be used");
			return false;
		}
	}
	return true;
}
