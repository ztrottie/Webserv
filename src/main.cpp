#include <iostream>
#include "../include/color.h"
#include "../include/Webserv.hpp"
#include "../include/parsing.hpp"

int main(void) {
	std::cout << YELLOW "Launching the webserv!" RESET << std::endl;
	Webserv webserv;
	parsing parsing("config.cfg");
	if (parsing.parseConfigFile() == false){
		writeTimestamp(RED, "ERROR IN PARSING QUITTING....");
		return 1;
	}
	// webserv.addNewServer(8080, "10.13.3.3", "serv1");
	// webserv.loop();
	return 0;
}
