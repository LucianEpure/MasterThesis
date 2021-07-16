#pragma once
#include "SourceReader.h"
#include "FiniteStateMachine.h"
#include "CodeParser.h"
#include<vector>
#include<string>
class FiniteStateExtractor
{
public:
	FiniteStateExtractor();
	std::vector<FiniteStateMachine> getFiniteStateMachines(std::vector<std::string> inputData);
	std::vector<std::string> getData(std::string inputFile);
};

