#pragma once
#include<string>
#include"Constants.h"
class Variab
{
public:
	Variab();

	std::string name;
	std::string type;
	std::string location;
	bool isEnum;
	bool isInt;

	std::string getProgramVariable();
};
