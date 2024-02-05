#pragma once
#include <string>
#include "color.h"
#include <iostream>
#include <vector>

std::string					timestamp();
void						writeTimestamp(std::string color, std::string const &message);
bool						containsNonDigit(const std::string& str);
bool						checkIdentationParsing(std::string const &firstWord, int nbIndentation, std::string const &line, bool defaultIfError, std::string const &lineName);
bool						isThereSomethingInMyString(std::string const &line, std::string const &begin, bool _defaultIfError, bool inLocation, bool checkNumber);
std::string					retIsThereSomethingInMyString(std::string const &input, std::string const &beginning, std::string const& end, bool erase);
std::vector<std::string>	splitString(std::string const &input, char delimiter);
bool						verifyAllowedMethods(std::string const &line);
bool						checkVargule(std::string const &line, bool _defaultIfError, bool insideLocation);