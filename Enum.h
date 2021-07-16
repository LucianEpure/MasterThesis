#pragma once
#include<string>
#include<vector>
#include<map>
#include"EnumElement.h"
#include"Constants.h"
class Enum
{
public:
	Enum();

	std::string enumName;
	std::vector<EnumElement> enumElements;
};

