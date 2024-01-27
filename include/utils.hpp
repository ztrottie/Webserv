#pragma once
#include <string>
#include "color.h"

std::string timestamp();
void	writeTimestamp(std::string color, std::string const &message);
bool	isThereSomethingInMyString(std::string const &input, std::string const &beginning, std::string const&end, bool write);