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
	std::string _header;
	std::string _fullResponse;
	std::string _headGenerator() const;
	std::string _footerGenerator() const;
	std::string _linkGenerator(std::string const &name) const;

public:
	// Constructors / Destructor
	Response();
	Response(Request *request, int flag);
	Response(const Response &inst);
	~Response();

	// Operator Overload
	Response& operator=(const Response &rhs);

	// Functions
	const std::string & getFullResponse() const;
	void setBody(std::string const & body);
	int openPath(std::string const &path);

	void internalServerError(int & errorCode);
	void contentTypeGenerator(std::string const &path);
	void handleGet(Request *request, int &errorCode);
	void handleDelete(Request *request, int &errorCode);
	void handlePost(Request *request, int &errorCode);
	void handleUploadedFile(Request *request, int &errorCode);
	void handleCgi(Request *request, int &errorCode);
	void directoryListing(Request *request, int &errorCode);
	void headerGenerator(int &errorCode, Request *request, int flag);
	void codeMessage(int code, std::string &message);
};
