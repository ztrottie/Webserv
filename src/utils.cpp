#include "../include/utils.hpp"

std::string	timestamp() {
	std::time_t currentTime = std::time(nullptr);
	std::tm* localTime = std::localtime(&currentTime);
	char timestampValue[21];
	std::strftime(timestampValue, sizeof(timestampValue), "[%Y-%m-%d %H:%M:%S]", localTime);
	timestampValue[sizeof(timestampValue) - 1] = '\0';
	std::string tmp = timestampValue;
	return (tmp);
}

void	selectMessage(int flags, int errorCode, unsigned int nbLine, std::string messSupp){
	std::string	color;
	switch (flags){
		case WRONG:
			color = RED;
			break;
		case DEFAULT:
			color = YELLOW;
			break;
		case VALID:
			color = GREEN;
			break;
	}
	std::string message = enumToString(errorCode);
	writeTimestamp(color, message, nbLine, messSupp);
}

std::string	enumToString(int code){
	switch (code) {
		case IDENTATIONERROR: return "This line has an error with it's identation";
		case IMPOSTORERROR: return "This line has some arguments that are not wanted";
		case VARGULEERR: return "This line must finish with an \";\"";
		case NUMBERARGSERROR: return "This line must have some arguments";
		case DIGITERROR: return "This line as an error with an argument that must only contain digit";
		case SPACEERROR: return "This line must only contain space between the arguments";
		case NOVARGULE: return "This line must not finish with \";\"";
		case MISSINGEND: return "This line doesn't have the right character at the end";
		case BADARGS: return "This line have an arguments that is not valid";
		case WRONGRETURN: return "This return line must need as it's last argument an path, https, error_code or a string";
		case WRONGMETHODS: return "This allowedMethods line can only have GET, POST, DELETE as arguments";
		case NUMBERTOOBIG: return "This number does not fit in the variable it is assigned";
		case NUMBERTOOLOW: return "This number is too low, it must be at least: ";
		case NOERR: return "This line is ok";
	}
	return "Unknowed error.... HELP";
}

void writeTimestamp(std::string color, std::string const &message, unsigned int nbLine, std::string const &messSupp){
	std::cout << color << "[" << RESET << std::to_string(nbLine + 1) << color << "]" << timestamp() << " " << message << messSupp << RESET << std::endl;
}

void simpleWriteTimestamp(std::string color, std::string const &message){
	std::cout << color << timestamp() << " " << message << RESET << std::endl;
}

bool containsNonDigit(const std::string& str){
	for (size_t i = 0; i < str.length(); ++i){
		if (!isdigit(str[i]) && str[i] != ';' && str[i] != ' ' && str[i] != '	' && str[i] != '\n'){
			return true;
		}
	}
	return false;
}

// std::string retIsThereSomethingInMyString(std::string const &input, std::string const &beginning, std::string const& end, bool erase){
// 	size_t start;
// 	size_t finish;
// 	std::string impostor;

// 	start = input.find(beginning);
// 	finish = input.find(end);
// 	impostor = input.substr(start + beginning.length(), (finish - (start + beginning.length())));
// 	if (erase == true)
// 		impostor.erase(std::remove_if(impostor.begin(), impostor.end(), ::isspace), impostor.end());
// 	if (impostor.length() > 0){
// 		return impostor;
// 	}
// 	return impostor;
// }

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

bool	verifyAllowedMethods(std::string &line){
	string tempLine = line;
	tempLine.erase(0, 16);
	std::vector<std::string> res = splitString(tempLine, ',');
	for (size_t i = 0 ; i < res.size(); i++){
		res[i].erase(std::remove_if(res[i].begin(), res[i].end(), ::isspace), res[i].end());
		if (res[i].find(";") != string::npos)
			res[i].erase(res[i].length() - 1, res[i].length());
	}
	for (size_t i = 0 ; i < res.size(); i++){
		if (res[i] == "GET")
			continue;
		else if (res[i] == "POST")
			continue;
		else if (res[i] == "DELETE")
			continue;
		else
			return false;
	}
	return true;
}

bool checkIdentationLocation(std::string const &line){
	return ((line[0] != '	' || line[1] != '	') ? false : true);
}
