#include <iostream>
#include "../include/color.h"
#include "../include/Webserv.hpp"

int main(void) {
	std::cout << YELLOW "Launching the webserv!" RESET << std::endl;
	Webserv webserv;
	webserv.addNewServer(8080, "10.13.3.3", "serv1");
	webserv.loop();
    return 0;
}
