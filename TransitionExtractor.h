#pragma once
#include<vector>
#include<string>
#include"FiniteStateMachine.h"
#include"Assig.h"
#include"Constraint.h"
#include"Enum.h"
#include"Loopp.h"
#include"ConstraintSolver.h"
#include"TransitionFunction.h"
#include"Path.h"
#include"Constants.h"

class TransitionExtractor
{
public:
	TransitionExtractor();
	TransitionExtractor(TransitionExtractor& s)
	{

	}
	std::vector<Assig> assignments;
	std::vector<Variab> programVariables;
	std::vector<Enum> enums;
	ConstraintSolver  constraintSolver;

	void checkIfAssigsInsideLoop(std::vector<std::string> sourceData, Loopp loop);
	std::vector<TransitionFunction> extractTransitions(std::vector<std::string> sourceData, FiniteStateMachine finiteStateMachine);
	std::vector<std::string> getCodeBeforeLoop(std::vector<std::string> sourceData, Loopp loop);
	std::vector<std::string> getCodeAfterLoop(std::vector<std::string> sourceData, Loopp loop);
	std::vector<Constraint> getConstraints(std::vector<std::string> sourceData, Loopp loop, Assig assig1, Assig assig2, bool& noPathPossible);
	std::string convertCondition(std::string condition);
	bool checkConstraints(std::vector<Constraint> constraints);
	std::vector<Constraint> extractFromIf(std::vector<std::string> dataContent, Assig assig1, Assig assig2, int loopPoint);
	std::vector<Constraint> extractFromSwitch(std::vector<std::string> dataContent, Assig assig1, Assig assig2, int loopPoint);
	std::vector<Constraint> addInitialConstraint(std::vector<Constraint> constraints, Assig assig1);
};

