#include "../include/utils.hpp"
#include <algorithm>
#include <string>
#include <vector>


bool isThereSomethingInMyString(std::string line, std::string const &begin, size_t finish){
	std::string impasta;
	size_t start;
	start = line.find(begin);
	impasta = line.substr(start + begin.length(), (finish - (start + begin.length())));
	impasta.erase(std::remove_if(impasta.begin(), impasta.end(), ::isspace), impasta.end());
	if (impasta.length() > 0)
		return true;
	return false;
}

bool	checkIdentationParsing(std::string const &line, std::string const &firstWord, bool isInsideLocation){
	size_t nbIdentation = 1;
	if (isInsideLocation == true)
		nbIdentation = 2;
	if (line.find(firstWord) != nbIdentation){
		return false;
	}
	if (line[0] != '	'){
		return false;
	}else if(isInsideLocation == true){
		if (line[1] != '	'){
			return false;
		}
	}
	return true;
}

string parsing::findFirstWord(string line){
	int	start = line.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	if (start == -1)
		return "";
	int	end = line.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_", start);
	if (end == -1)
		end = line.length();
	string res = line.substr(start, end - start);
	return res;
}

bool	checkVargule(std::string const &line){
	if (line.find(";") != line.length() - 1){
		return false;
	}else if (line[line.length() - 2] == ' '){
		return false;
	}
	return true;
}

bool	checkForTabs(string const &line, size_t lastRequireTabs){
	if (line.rfind("	") != lastRequireTabs){
		return false;
	}
	return true;
}

void	wagadooMachine(string const &line, int flags, int errorCode, size_t nbLine, string messSupp, bool defaultIfError, std::vector<int> &verifLine, bool insideLocation){
	if (insideLocation == true){
		selectMessage(flags, errorCode, nbLine, messSupp + ", in the line \"" + line + "\", this line will not be used");
			if (nbLine == verifLine.size())
				verifLine.push_back(DONT);
		return ;
	}
	if (defaultIfError == false){
			selectMessage(flags, errorCode, nbLine, messSupp + ", in the line \"" + line + "\"");
			verifLine.push_back(DONT);
		}else {
			selectMessage(flags, errorCode, nbLine, messSupp + ", in the line \"" + line + "\" switching to default");
			if (nbLine == verifLine.size())
				verifLine.push_back(SWITCH);
		}
}

bool	checkForArgs(string const &line, size_t minSizeExpected){
	string str = line;
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
	if (str.length() < minSizeExpected){
		return false;
	}
	return true;
}

bool	checkErrorCode(string const &line){
	string tempLine = line;
	if (tempLine.find(";") < string::npos)
		tempLine.erase(tempLine.length() - 1, tempLine.length());
	switch (std::stoi(tempLine)){
		case 200: return true;
		case 201: return true;
		case 202: return true;
		case 204: return true;
		case 205: return true;
		case 206: return true;
		case 300: return true;
		case 301: return true;
		case 302: return true;
		case 303: return true;
		case 304: return true;
		case 305: return true;
		case 307: return true;
		case 401: return true;
		case 403: return true;
		case 404: return true;
		case 405: return true;
		case 406: return true;
		case 413: return true;
		default: return false;
	}
	return false;
}

bool	locationAlreadyThere(const string &allLocation, string const &name){
	std::vector<string> split = splitString(allLocation, '	');
	for (size_t i = 0; i < split.size(); i++) {
		if (!split[i].empty())
			if (split[i] == name)
				return true;
	}
	return false;
}