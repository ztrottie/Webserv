#include <iostream>
#include "../include/color.h"
#include "../include/Server.hpp"
#include "../include/parsing.hpp"

int main(void) {
	std::cout << YELLOW "Launching the webserv!" RESET << std::endl;
	parsing parsing("config.cfg");
	if (parsing.parseConfigFile() == false)
		return 1;
	// Server defaultServer;
	return 0;
}
