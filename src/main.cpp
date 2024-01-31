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
	std::string path;
	Router bob;
	Location *bobshouse = new Location();
	bob.addLocation("/", bobshouse);
	bob.addLocation("/bonjour", bobshouse);
	bob.getFile("GET", "/bonjour/slaut", path);
	bob.addAllowedMethod("GET");
    return 0;
}
