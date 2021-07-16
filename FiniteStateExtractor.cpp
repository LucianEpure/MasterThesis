#include <iostream>
#include <vector>
#include <string>
#include "FiniteStateExtractor.h"

FiniteStateExtractor::FiniteStateExtractor()
{

}

std::vector<FiniteStateMachine> FiniteStateExtractor::getFiniteStateMachines(std::vector<std::string> inputData)
{
	CodeParser codeParser;
	std::vector<FiniteStateMachine> finiteStateMachines = codeParser.parseCode(inputData);
	return finiteStateMachines;
}

std::vector<std::string> FiniteStateExtractor::getData(std::string inputFile)
{
	std::vector<std::string> data;
	SourceReader sourceReader;
	data = sourceReader.readFromSource(inputFile.c_str());

	return data;
}