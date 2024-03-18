#pragma once
#include <cstddef>
#include <map>
#include <string>
#include <vector>

class Router;

class Location
{
private:
	const std::string 			_name;
	std::string					_root;
	std::string					_index;
	std::map<int, std::string>	_errorPagesLocation;
	std::string					_rewrite;
	std::vector<std::string>	_allowedMethod;
	long long					_clientMaxBodySize;
	bool						_autoIndex;
	bool						_autoIndexSetter;
	bool						_uploadEnable;
	bool						_uploadEnableSet;
	std::string					_uploadStore;
	bool						_redirection;
	int 						_redirectionCode;
	std::string					_redirectionLocation;
	bool						_clientMaxBodySizeSet;
	bool						_redirIsString;
	bool						_useCGI;
	bool						_useCGISetter;

public:
	// Constructors / Destructor
	Location();
	Location(std::string const &name);
	Location(const Location &inst);
	~Location();

	//Setters
	void setUploadEnable(bool enable);
	void setRoot(std::string const &root);
	void setIndex(std::string const &index);
	void setAutoIndex(bool const &autoIndex);
	void addErrorPage(const int errorNumber, std::string pathToError);
	void addAllowedMethod(std::string const &method);
	void setUploadStore(std::string const &value);
	void setRedirection(bool value);
	void setRedirectionCode(int code);
	void setRedirectionLocation(std::string str, bool isString);
	void setClientMaxBodySize(size_t value);
	void setUseCGI(bool value);

	// Functions
	int isMethodAllowed(std::string const &method);
	int isErrorCodeValid(int const &errorCode, std::string &path);

	const std::string	&getUploadStore() const;
	const std::string	&getName() const;
	int					getRoot(std::string &root);
	bool				getUploadEnable() const;
	bool				getAutoIndex() const;
	bool				getRedirection() const;
	int					getRedirectionCode() const;
	std::string			getRedirectionLocation() const;
	void 				getClientMaxBodySize(size_t &clientMaxBodySize) const;
	std::string			getAllowedMethods() const;
	std::string			getIndex() const;
	bool				getUseCGI() const;
	std::string			getRootParsing() const;
	std::string			getErrorParsing(int code) const;
	long long			getClientMaxBodySizeParsing() const;
	bool				getUploadEnableSetter() const;
	bool				getAutoIndexSetter() const;
	bool				getUseCGISetter() const;
};

// Location_HPP