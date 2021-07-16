#include<iostream>
#include "SourceReader.h"
SourceReader::SourceReader()
{
}


std::vector<std::string> SourceReader::readFromSource(const char* sourceName)
{
	std::vector<std::string> data;
	std::ifstream file(sourceName);
	std::string str;
	while (std::getline(file, str)) {
		data.push_back(str);
	}
	return data;
}