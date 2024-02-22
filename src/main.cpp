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
	Location *upload = new Location();
	upload->setUploadEnable(true);
	upload->setAutoIndex(false);
	upload->setIndex("");
	upload->setRoot("./uploads");
	upload->setClientMaxBodySize(10000);
	loc->setAutoIndex(true);
	std::cout << loc->getAutoIndex() << std::endl;
	bob->addAllowedMethod("GET");
	bob->addAllowedMethod("POST");
	bob->setIndex("index.html");
	bob->setRoot("./www");
	bob->addErrorPage(404, "./www/errors/404.html");
	bob->addLocation("/", loc);
	webserv.addNewServer(8080, "10.11.1.3", "default", bob, 3000);
	webserv.loop();
	return 0;
}
