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
	Router *bob = new Router();
	Location *loc = new Location("/");
	Location *upload = new Location("/uploads");
	upload->setUploadEnable(true);
	upload->setUploadStore("./uploads");
	upload->setAutoIndex(false);
	upload->setIndex("");
	upload->setRoot("./uploads");
	upload->setClientMaxBodySize(10000);
	loc->setAutoIndex(true);
	bob->addAllowedMethod("GET");
	bob->addAllowedMethod("POST");
	bob->setIndex("index.html");
	bob->setRoot("./www");
	bob->addErrorPage(404, "./www/errors/404.html");
	bob->addLocation("/", loc);
	bob->addLocation("/uploads", upload);
	webserv.addNewServer(8080, "10.11.1.3", "default", bob, 3000);
	webserv.loop();
	return 0;
}
