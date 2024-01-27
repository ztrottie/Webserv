#pragma once

#include <string>
#include <iostream>
#include <sys/stat.h>
#include "color.h"
#include "utils.hpp"

#define DEFAULT

enum flags{
	CORRECT,
	WRONGPATH,
	DIR,
	WRONGEXT
};

using std::string;
using std::cout;
using std::endl;



class parsing{
	private:
		string	pathConfigFile;
		bool	defaultIfError;
		void	error(int errorCode);
		bool	checkFile();

	public:
		parsing(string path);
		bool parseConfigFile();
		~parsing();
};
