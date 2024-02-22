#include <iostream>
#include "../include/color.h"
#include "../include/Webserv.hpp"
#include "../include/parsing.hpp"

int main(void) {
	std::cout << YELLOW "Launching the webserv!" RESET << std::endl;
	parsing parsing("config.cfg");
	if (parsing.parseConfigFile() == false){
		simpleWriteTimestamp(RED, "ERROR IN PARSING QUITTING....");
		return 1;
	}
	parsing.assignConfigFile();
	// Router *bob = new Router();
	// Location *loc = new Location();
	// bob->addAllowedMethod("GET");
	// bob->addAllowedMethod("POST");
	// bob->setIndex("index.html");
	// bob->setRoot("./www");
	// bob->addErrorPage(404, "./www/erros/404.html");
	// bob->addLocation("/", loc);
	// parsing.webserv.loop
	return 0;
}
