#include "FiniteStateMachine.h"

FiniteStateMachine::FiniteStateMachine()
{


}

void FiniteStateMachine::displayMachine()
{
	std::cout << "\n Finite State Machine: " << this->name;

	std::cout << "\n State Variable: " << this->stateVar.name;

	std::cout << "\n States:";
	for (int i = 0; i < this->states.size(); i++)
	{
		std::cout << this->states[i].stateName << " ";
	}
	std::cout << "\n Transitions:";
	for (int i = 0; i < this->transitionFunctions.size(); i++)
	{
		std::cout << "\n" << i << " FROM " << this->transitionFunctions[i].startState.stateName << " TO " << this->transitionFunctions[i].finishState.stateName;
	}
	std::cout << "\n Initial State: " << this->initialState.stateName;

	std::cout << "\n Final States: ";

	for (int i = 0; i < this->finalStates.size(); i++)
	{
		std::cout << this->finalStates[i].stateName << " ";
	}


}