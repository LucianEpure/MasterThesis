#pragma once
#include<vector>
#include<iostream>
#include"State.h"
#include"Variab.h"
#include"Loopp.h"
#include"Constraint.h"
#include"TransitionFunction.h"
class FiniteStateMachine
{
public:
	FiniteStateMachine();

	std::string name;
	std::vector<State> states;
	std::vector<TransitionFunction> transitionFunctions;
	State initialState;
	std::vector<State> finalStates;
	Variab stateVar;
	Loopp loop;

	void displayMachine();
};

