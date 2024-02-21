#pragma once
#include <string>
#include "Request.hpp"
#include "Location.hpp"


class Response
{
private:
	std::string _path;
	std::string _body;

public:
	// Constructors / Destructor
	Response(Request *request, Router *router, Location *location, int &errorCode);
	Response(const Response &inst);
	~Response();

	// Operator Overload
	Response& operator=(const Response &rhs);

	// Functions
	const std::string & getBody() const;
	const std::string & getPath() const;

	void setPath(std::string const & path);
	void setBody(std::string const & body);
	int openPath(std::string const &path);
};
