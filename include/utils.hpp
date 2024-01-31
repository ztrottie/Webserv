#pragma once
#include <string>
#include "color.h"

std::string timestamp();
void	writeTimestamp(std::string color, std::string const &message);
bool	checkIdentationParsing(std::string const &firstWord, int nbIndentation, std::string const &line, bool inLocation, bool defaultIfError, std::string const &lineName);
bool	isThereSomethingInMyString(std::string const &input, std::string const &beginning, std::string const&end, bool write);