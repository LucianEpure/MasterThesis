#pragma once

#include<string>
#include"Constants.h"

class Constraint
{
public:
	Constraint();

	std::string constraintString;
	std::string elem1;
	std::string elem2;
	std::string op;
};
