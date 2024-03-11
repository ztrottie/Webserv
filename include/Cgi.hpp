#pragma once

#include "Router.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include "Response.hpp"
#include "Client.hpp"

class Cgi
{
private:

	const char *_env[];

public:
	// Constructors / Destructor
	Cgi();
	Cgi(const Cgi &inst);
	~Cgi();

	// Operator Overload
	Cgi& operator=(const Cgi &rhs);

	// Functions
	char** execute(Request *request);
	void env(Request *request, Router *router);

};

// Cgi_HPP