#pragma once
#include <string>
#include "Request.hpp"
#include "Location.hpp"

#include "Request.hpp"
#include "Router.hpp"

class Response
{
private:
	std::string _contentType;
	std::string _body;
	std::string _headGenerator() const;
	std::string _footerGenerator() const;
	std::string _linkGenerator(std::string const &path, std::string const &name) const;

public:
	// Constructors / Destructor
	Response(Request *request, Router *router, Location *location, int &errorCode);
	Response(const Response &inst);
	~Response();

	// Operator Overload
	Response& operator=(const Response &rhs);

	// Functions
	const std::string & getBody() const;

	void setBody(std::string const & body);
	int openPath(std::string const &path);

	void internalServerError(int & errorCode);
	void contentTypeGenerator(std::string const &path);
	void handleGet(Request *request, Router *router, Location *location, int &errorCode);
	void handleDelete(Request *request, Router *router, Location *location, int &errorCode);
	void directoryListing(Request *request, int &errorCode);
};
