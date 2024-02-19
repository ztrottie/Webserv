#include "../include/parsing.hpp"

// client_max_body_size 1B; = 1
// client_max_body_size 1K; = 1000
// client_max_body_size 1M; = 1 000 000
// client_max_body_size 1G; = 1 000 000 000

void parsing::assignConfigFile(){
	string line;
	std::ifstream file;

	file.open(pathConfigFile);
	for (size_t i = 0; i < verifLine.size(); i++) {
		std::getline(file, line);
		cout << "line " << i + 1 << "	" << line << " : " << verifLine[i] << endl;
	}
	// Router *rout = new Router();
	// rout->setRoot(const std::string &root);
	// rout->setIndex(const std::string &index);
	// rout->addAllowedMethod(const std::string &method);
	// rout->addErrorPage(const int errorNumber, std::string pathToError);
	//si location
	// Location *loc = new Location();
	// loc->addAllowedMethod(const std::string &method);
	// loc->addAllowedMethod(const std::string &method);
	// loc->addAllowedMethod(const std::string &method);
	// loc->addErrorPage(const int errorNumber, std::string pathToError);
	// loc->setIndex(const std::string &index);
	// loc->setRoot(const std::string &root);
	// rout->addLocation(const std::string &key, Location *loc);
	// Server *serv = new Server();
}

// void parsing::assignMaxBody(const string &line, Router &rout){

// }

// void parsing::assignRoot(const string &line, Router &rout){

// }

// void parsing::assignIndex(const string &line, Router &rout){

// }

// void parsing::assignErrorPage(const string &line, Router &rout){

// }

// void parsing::assignAllowedMethods(const string &line, Router &rout){

// }

// void parsing::assignReturn(const string &line, Router &rout){

// }
