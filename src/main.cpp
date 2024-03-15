#include <iostream>
#include "../include/color.h"
#include "../include/Webserv.hpp"
#include "../include/parsing.hpp"

int main(int argc, char **argv){
	if (argc != 2){
		cout << "Error, you must provide a config file" << endl;
		return 1;
	}
	std::cout << YELLOW "Launching the webserv!" RESET << std::endl;
	Webserv webserv;
	parsing parsing(argv[1]);
	if (parsing.parseConfigFile() == false){
		simpleWriteTimestamp(RED, "ERROR IN PARSING QUITTING....");
		return 1;
	}
	parsing.assignConfigFile(&webserv);
	// webserv.loop();
	return 0;
}
