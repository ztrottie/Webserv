#include <iostream>
#include "../include/color.h"
#include "../include/Webserv.hpp"
#include "../include/parsing.hpp"

int main(void) {
	std::cout << YELLOW "Launching the webserv!" RESET << std::endl;
	Webserv webserv;
	parsing parsing("config.cfg");
	if (parsing.parseConfigFile() == false)
		return 1;
	webserv.addNewServer(8081, "10.12.6.4", "default", NULL);
	webserv.loop();
    return 0;
}
