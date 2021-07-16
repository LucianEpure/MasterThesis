#pragma once
#include<vector>
#include<string>
#include<iostream>
#include<sstream>   
#include<map>
#include"FiniteStateMachine.h"
#include"Variab.h"
#include"Loopp.h"
#include"Enum.h"
#include"Funct.h"
#include"TransitionExtractor.h"
#include"Constants.h"
#include"TransitionFunction.h"

class CodeParser
{
public:
	CodeParser();

	std::vector<std::string> possibleVarTypes{ "int","bool","char","float","double","char*","int*" };
	std::vector<Enum> enums;
	std::vector<std::string> possibleExCondTypes{ "int","char","int*","char*" };
	std::vector<std::string> forbiddenCharact{ "(", ")",";","{","}" };
	std::vector<Variab> loopVariables;
	std::vector<Variab> programVariables;
	std::vector<Funct> programFunctions;
	TransitionExtractor transitionExtractor;

	std::vector<FiniteStateMachine> parseCode(std::vector<std::string> sourceData);
	//bool checkIfVarEx

private:
	std::vector<Loopp> findLoops(std::vector<std::string> sourceData);
	void getVariabs(std::vector<std::string> sourceData);
	bool checkIfVariableInList(std::string variable);
	std::vector<Variab> checkIfVarInExitCondVars(std::string varToCheck, std::string varOp);
	bool checkIfVariableInExitCondList(std::string variable, std::vector<Variab> exitCond);
	bool checkIfVariableIterated(std::vector<std::string> allLine, int varPos);
	std::vector<Variab> obtainStateVariables(Loopp loop);
	bool parseAndCheckIfState(Loopp loop, Variab variab);
	std::vector<State> obtainStates(Loopp loop, Variab stateVar);
	void fillFunctions(std::vector<std::string> sourceData);


	State getInitialState(FiniteStateMachine finiteStateMachine, std::vector<std::string> sourceCode);
	std::vector<State> getFinalStates(FiniteStateMachine finiteStateMachine, std::vector<std::string> sourceCode);

};


