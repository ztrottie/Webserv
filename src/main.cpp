#include <iostream>
#include "../include/color.h"
#include "../include/Webserv.hpp"
#include "../include/parsing.hpp"

int main(void) {
	// std::cout << YELLOW "Launching the webserv!" RESET << std::endl;
	// Webserv webserv;
	// parsing parsing("config.cfg");
	// if (parsing.parseConfigFile() == false)
	// 	return 1;
	// webserv.addNewServer(8081, "127.0.0.1", "default", NULL);
	// webserv.loop();
	// std::string path;
	Router *bob = new Router();
	Location *loc = new Location();
	Webserv	webserv;
	bob->addAllowedMethod("GET");
	bob->setIndex("index.html");
	bob->setRoot("./www");
	bob->addErrorPage(404, "./www/erros/404.html");
	bob->addLocation("/", loc);
	webserv.addNewServer(8080, "10.13.3.4", "default", bob);
	webserv.loop();
    return 0;
}
