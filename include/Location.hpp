#pragma once
#include <map>
#include <string>

class Location
{
private:
	const std::string			_root;
	const std::string			_index;
	std::map<int, std::string>	_errorPagesLocation;
	const std::string			_rewrite;
public:
	// Constructors / Destructor
	Location();
	Location(const Location &inst);
	~Location();

	// Operator Overload
	Location& operator=(const Location &rhs);

	// Functions

};

// Location_HPP