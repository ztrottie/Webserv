#include <iostream>
#include "../include/color.h"
#include "../include/Webserv.hpp"
#include "../include/parsing.hpp"

int main(void) {
	// webserv.addNewServer(8081, "127.0.0.1", "default", NULL);
	// webserv.loop();
	// std::string path;
	std::cout << YELLOW "Launching the webserv!" RESET << std::endl;
	Webserv webserv;
	parsing parsing("config.cfg");
	if (parsing.parseConfigFile() == false){
		writeTimestamp(RED, "ERROR IN PARSING QUITTING....");
		return 1;
	}
	Router *bob = new Router();
	Location *loc = new Location();
	bob->addAllowedMethod("GET");
	bob->addAllowedMethod("POST");
	bob->setIndex("index.html");
	bob->setRoot("./www");
	bob->addErrorPage(404, "./www/erros/404.html");
	bob->addLocation("/", loc);
	webserv.addNewServer(8080, "10.12.5.9", "default", bob, 3000);
	webserv.loop();
	return 0;
}
