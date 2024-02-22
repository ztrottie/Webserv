#include "../include/utils.hpp"
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
		// cout << "ah bon?" << endl;
		return false;
	}
	if (line[0] != '	'){
		// cout << "ah oui?" << endl;
		return false;
	}else if(isInsideLocation == true){
		if (line[1] != '	'){
			// cout << "meeehhh?" << endl;
			return false;
		}
	}
	return true;
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
