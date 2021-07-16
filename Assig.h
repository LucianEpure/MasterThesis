#pragma once
#include<string>
#include"Variab.h"
#include"State.h"
#include"Constants.h"
//class to mark the assignment of the state variable to a state
class Assig
{
public:
	Assig();

	Variab stateVar;
	State assignedState;
	bool insideLoop;
	std::string assigText;
};

