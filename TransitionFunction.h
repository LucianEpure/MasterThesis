#pragma once
#include<string>
#include<vector>
#include"State.h"
#include"Constraint.h"
#include"Constants.h"
class TransitionFunction
{
public:
	TransitionFunction();

	State startState;
	State finishState;
	std::string transitionVal;
};

