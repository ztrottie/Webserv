#include "../include/parsing.hpp"

parsing::parsing(string path): pathConfigFile(path){
}

bool parsing::parseConfigFile(){
	cout << PURPLE << "Commencement du parsing" << RESET << endl;
	if (checkFile() == false){
		return false;
	}
	return true;
}

bool	parsing::checkFile(){
	string			line;
	struct stat		fileStat;

	if (stat(pathConfigFile.c_str(), &fileStat) == 0){
		if (S_ISDIR(fileStat.st_mode) == true){
			error(DIR);
			return DIR;
		}
	}
	else {
		error(WRONGPATH);
		return WRONGPATH;
	}
	if (pathConfigFile.find(".cfg") == string::npos){
		error(WRONGEXT);
		return WRONGEXT;
	}
	cout << timestamp() << GREEN << "Tout est correct pour le path" << RESET << endl;
	return true;
}

void	parsing::error(int errorCode){
	switch (errorCode){
		case 1:
			cout << timestamp() << RED << "Le fichier d'entré est non existant" << RESET << endl;
			break;
		case 2: 
			cout << timestamp() << RED << "Le \"Fichier\" n'en est pas un, c'est un dossier" << RESET << endl;
			break;
		case 3:
			cout << timestamp() << RED << "Le fichier d'entré n'a pas la bonne extension, l'extension recherché est : \".cfg\"" << RESET << endl;
			break;
	}
}

parsing::~parsing(){
}