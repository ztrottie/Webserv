#pragma once

#include "Request.hpp"
#include <cstddef>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

class Cgi
{
private:

	int			_outputFd;
	const char 		*_env[12];

public:
	// Constructors / Destructor
	Cgi();
	Cgi(const Cgi &inst);
	~Cgi();

	// Operator Overload
	Cgi& operator=(const Cgi &rhs);

	// Functions
	void env(Request *request);
};

// Cgi_HPP