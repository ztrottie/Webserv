#pragma once
#include <string>
#include "color.h"
#include "parsing.hpp"
#include <iostream>
#include <vector>

class parsing;

enum flags{
	WRONG,
	DEFAULT,
	VALID
};

enum errorParsing{
	IDENTATIONERROR,
	IMPOSTORERROR,
	VARGULEERR,
	NUMBERARGSERROR,
	DIGITERROR,
	SPACEERROR,
	NOVARGULE,
	MISSINGEND,
	BADARGS,
	WRONGRETURN,
	WRONGMETHODS,
	NOERR
};

using std::string;
using std::cout;
using std::endl;

std::string					timestamp();
std::string					enumToString(int code);
void						selectMessage(int flags, int errorCode, unsigned int nbLine, std::string messSupp);
void						writeTimestamp(std::string color, std::string const &message, unsigned int nbLine, std::string const &messSupp);
void						simpleWriteTimestamp(std::string color, std::string const &message);
bool						containsNonDigit(const std::string& str);
// std::string					retIsThereSomethingInMyString(std::string const &input, std::string const &beginning, std::string const& end, bool erase);
std::vector<std::string>	splitString(std::string const &input, char delimiter);
bool						verifyAllowedMethods(std::string &line);
bool						checkIdentationLocation(const std::string &line);

//parsing
bool	checkIdentationParsing(std::string const &line, std::string const &firstWord, bool isInsideLocation);
bool	isThereSomethingInMyString(std::string line, std::string const &begin, size_t finish);
bool	checkForTabs(string const &line, size_t lastRequireTabs);
bool	checkVargule(std::string const &line);
bool	checkForArgs(string const &line, size_t minSizeExpected);
void	wagadooMachine(string const &line, int flags, int errorCode, size_t nbLine, string messSupp, bool defaultIfError, std::vector<int> &verifLine, bool insideLocation);