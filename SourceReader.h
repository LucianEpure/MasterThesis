#pragma once
#include<vector>
#include<string>
#include<fstream>

#include"Constants.h"

class SourceReader
{
public:
	SourceReader();

	std::vector<std::string> readFromSource(const char* sourceName);
};

