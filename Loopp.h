#pragma once
#include<string>
#include<vector>
#include<iostream>
#include<sstream>   
#include"Variab.h"
#include"Funct.h"
#include"Constants.h"
class Loopp
{
public:
	Loopp();
	std::string loopType;
	bool constantCount;
	std::vector<Variab> exitCondVars;
	std::string exitCondStatement;
	std::vector<std::string> loopContent;
	std::vector<Variab> loopVars;
	std::vector<Variab> possibleStateVars;
	Funct functionContainingLoop;

	void findLoopVariables(std::vector<Variab> programVariabs);
	bool checkIfAlreadyInVarList(std::string variable);
	void getPossibleStateVars();
};

