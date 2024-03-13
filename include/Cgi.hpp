#pragma once

#include "Request.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

class Cgi
{
private:

	int			_inputFd;
	int			_outputFd;
	const char 		*_env[100];

public:
	// Constructors / Destructor
	Cgi();
	Cgi(const Cgi &inst);
	~Cgi();

	// Operator Overload
	Cgi& operator=(const Cgi &rhs);

	// Functions
	void execute(Request *request);
	void env(Request *request);

};

// Cgi_HPP