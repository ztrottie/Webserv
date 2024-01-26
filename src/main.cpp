#include <iostream>
#include "../include/color.h"
#include "../include/Webserv.hpp"

int main(void) {
	std::cout << YELLOW "Launching the webserv!" RESET << std::endl;
	Webserv webserv;
	webserv.addNewServer(8080, "10.12.6.9", "serv1");
	webserv.loop();
    return 0;
}
