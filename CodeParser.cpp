#include "CodeParser.h"

CodeParser::CodeParser()
{
}


//function that obtains the variables from the source code
void CodeParser::getVariabs(std::vector<std::string> sourceData)
{
	bool defineEnum = false;
	bool defineVar = false;
	bool defineFunc = false;
	bool defineParam = false;
	bool defineStruct = false;
	bool isFunction = false;
	bool isVariab = false;
	int enumCount = 0;
	std::vector<std::string> line;
	std::vector<Variab> variabs;
	for (int i = 0; i < sourceData.size(); i++) {
		line.clear();
		std::istringstream ss(sourceData[i]);
		std::string word; // for storing each word

		while (ss >> word)
		{
			line.push_back(word);
		}
		//std::cout << line[0] <<"\n";

		if ((line.size()>0 && (line[0].compare("enum") == 0)) || (line.size()> 1 && (line[0].compare("typedef") == 0) && (line[1].compare("enum") == 0)))
		{
			enumCount = i;
			Enum enumeration;
			bool foundName = false;
			std::string word2;
			std::vector<std::string> line2;
			//std::cout << line[1] <<"\n";
			defineEnum = true;
			std::string enumOpt;

			//go in enum until find semicolon
			while (!foundName)
			{
				enumCount++;
				if (sourceData[enumCount].find(";") != std::string::npos)
				{
					foundName = true;
				}
				else
				{
					if (sourceData[enumCount].find(",") != std::string::npos)
					{
						enumOpt = sourceData[enumCount].substr(0, sourceData[enumCount].size() - 1);
					}
					else
					{
						enumOpt = sourceData[enumCount];
					}
					std::vector<std::string> line2;
					std::istringstream ss(enumOpt);
					std::string word; // for storing each word
					//std::cout << sourceData[i] << "\n";
					EnumElement elem;
					while (ss >> word)
					{
						line2.push_back(word);
					}
					elem.enumElName = line2[0];
					if (line2.size() > 1)
					{
						elem.enumElValue = std::stoi(line2[2]);
					}
					enumeration.enumElements.push_back(elem);
				}
			}
			std::istringstream ss2(sourceData[enumCount]);
			while (ss2 >> word2)
			{
				line2.push_back(word2);
			}
			possibleVarTypes.push_back(line2[1]);
			enumeration.enumName = line2[1];
			enums.push_back(enumeration);
			std::cout << enums[0].enumName << "\n\n\n\n";
			for (int k = 0; k < enumeration.enumElements.size(); k++)
			{
				std::cout << enumeration.enumElements[k].enumElName << "\n\n\n";
			}
		}
		else
		{
			isFunction = false;
			for (int k = 0; k < possibleVarTypes.size(); k++)
			{
				//std::cout << possibleVarTypes[k] << " ";
				//check if the word is of variable/funct type
				if (line.size()>1 && line[0].compare(possibleVarTypes[k]) == 0)
				{
					//if it is, check if is funct or variable
					for (int j = 1; j < line.size(); j++) {
						if (line[j].find(")") != std::string::npos)
						{
							isFunction = true;
						}
					}

					if (!isFunction)
					{
						isVariab = true;
						Variab variab;
						//also check if the type is enum, if it is set flag
						for (int j1 = 0; j1 < enums.size(); j1++) {
							if (enums[j1].enumName.compare(line[0]) == 0)
							{
								//std::cout << "aaaaaaaaa";
								variab.isEnum = true;
							}
						}
						variab.type = possibleVarTypes[k];
						variab.name = line[1];
						variab.location = FREE_TYPE;
						programVariables.push_back(variab);

						//add pointer version if particular case
						if (
							(possibleVarTypes[k].compare("int*") == 0) ||
							(possibleVarTypes[k].compare("char*") == 0))
						{
							Variab variab1;
							variab1.type = possibleVarTypes[k];
							variab1.name = "*" + line[1];
							variab1.location = FREE_TYPE;
							programVariables.push_back(variab1);
						}

					}
					else {
						Funct funct;
						funct.functName = line[1];
						funct.returnType = line[0];
						this->programFunctions.push_back(funct);
						//it is the main function
						if (line[1].find("main") != std::string::npos)
						{

						}
						else
						{
							for (int i1 = 2; i1 < line.size(); i1++)
							{
								if (checkIfVariableInList(line[i1]))
								{
									Variab variab;
									for (int j1 = 0; j1 < enums.size(); j1++) {
										if (enums[j1].enumName.compare(line[0]) == 0)
										{
											variab.isEnum = true;
										}
									}
									variab.name = line[i1 + 1];
									variab.type = line[i1];
									variab.location = PARAMETER;
									programVariables.push_back(variab);

									//add pointer version if particular case
									if ((line[i1].compare("int*") == 0) ||
										(line[i1].compare("char*") == 0))
									{
										Variab variab1;
										variab1.type = line[i1];
										variab1.name = "*" + line[i1 + 1];
										variab1.location = FREE_TYPE;
										programVariables.push_back(variab1);
									}

								}
							}
						}
					}
				}
			}
		}
		if (defineEnum) {

		}
	}
}

// check if a variable is in the list of possible variable types
bool CodeParser::checkIfVariableInList(std::string variable)
{
	for (int k = 0; k < possibleVarTypes.size(); k++)
	{
		if (variable.compare(possibleVarTypes[k]) == 0)
		{
			return true;
		}
	}
	return false;
}

//check if variable exist in the exit condition of the loop
bool CodeParser::checkIfVariableInExitCondList(std::string variable, std::vector<Variab> exitCond)
{
	for (int k = 0; k < exitCond.size(); k++)
	{
		if (variable.compare(exitCond[k].name) == 0)
		{
			return true;
		}
	}
	return false;
}

bool CodeParser::checkIfVariableIterated(std::vector<std::string> allLine, int varPos)
{
	//check if inc/dec classical
	if ((allLine[varPos + 1].compare("++") == 0) || (allLine[varPos + 1].compare("+=") == 0) || (allLine[varPos + 1].compare("--") == 0) || (allLine[varPos + 1].compare("-=") == 0))
	{
		return true;
	}
	//check if inc dec longer
	else if ((allLine[varPos + 1].compare("=") == 0))
	{
		for (int i = varPos + 2; i < allLine.size(); i++)
		{
			if (allLine[i].find(allLine[varPos]) != std::string::npos) {
				return true;
			}
		}
	}
	else
	{
		return false;
	}
}

//checks if the variable in the exit conditions can be reducing var
std::vector<Variab> CodeParser::checkIfVarInExitCondVars(std::string varToCheck, std::string varOp)
{
	std::vector<Variab> exitCondVars;
	for (int j = 0; j < programVariables.size(); j++)
	{
		//find the var and compare with the type
		if ((varToCheck.compare(programVariables[j].name) == 0) &&
			((programVariables[j].type.compare("int") == 0) ||
			(programVariables[j].type.compare("char") == 0) ||
				(programVariables[j].type.compare("int*") == 0) ||
				(programVariables[j].type.compare("char*") == 0)))
		{
			//check if compared with const
			if ((varOp.compare("==") == 0) || (varOp.compare("!=") == 0) || (varOp.compare(">=") == 0) || (varOp.compare("<=") == 0) || (varOp.compare(">") == 0) || (varOp.compare("<") == 0))
			{
				exitCondVars.push_back(programVariables[j]);
			}
			else
			{
				std::cout << "Not a red var";
			}
		}
	}
	return exitCondVars;
}


std::vector<Loopp> CodeParser::findLoops(std::vector<std::string> sourceData)
{
	Funct currentFunct;
	int loopPointer;
	std::vector<std::string> line;
	std::vector<Loopp> loops;
	for (int i = 0; i < sourceData.size(); i++)
	{
		std::istringstream ss(sourceData[i]);
		std::string word; // for storing each word

		while (ss >> word)
		{
			line.push_back(word);
		}

		//check the function list to see where we are
		for (int i2 = 0; i2 < this->programFunctions.size(); i2++)
		{
			if (line.size()>1 && programFunctions[i2].functName.compare(line[1]) == 0)
			{
				currentFunct = programFunctions[i2];
			}
		}
		//std::cout << "Funct is:" << currentFunct.functName << "\n";
		//loop starts
		if (line.size()>1 && line[0].compare("while") == 0)
		{
			Loopp loop;
			loop.loopType = "while";
			loop.functionContainingLoop = currentFunct;
			for (int j = 1; j < line.size(); j++)
			{
				loop.exitCondStatement += " " + line[j];
			}
			//check exit condition
			std::vector<Variab> loopVariables;
			std::vector<Variab> possibleReducingVar;
			std::vector<std::string> line2;
			loopPointer = i;
			int loopCounter;
			// go on the while statement and check vars in exit cond are int type and compared to const
			for (int j = 1; j < line.size() - 1; j++)
			{
				loop.exitCondVars = checkIfVarInExitCondVars(line[j], line[j + 1]);
				if (!loop.exitCondVars.empty()) {
					for (int kk = 0; kk < loop.exitCondVars.size(); kk++)
					{
						std::cout << loop.exitCondVars[kk].getProgramVariable() << "\n";
					}
				}
			}

			loopCounter = 0;
			//std::cout << "point is:" << loopPointer << "\n";
			//if there are variables in the exitCond, compared to a constant that may be reduction vars
			if (loop.exitCondVars.size() > 0)
			{
				//go through the loop and check if the vars in the exit cond are reduction
				do
				{
					loopPointer++;
					if (sourceData[loopPointer].find("{") != std::string::npos)
					{
						loopCounter++;
					}
					if (sourceData[loopPointer].find("}") != std::string::npos)
					{
						loopCounter--;
					}
					std::istringstream ss(sourceData[loopPointer]);
					std::string word; // for storing each word

					while (ss >> word)
					{
						line2.push_back(word);
					}
					//see if the vari is in exit cond list and if it was itterated or not with constant value
					if (line2.size()>1 && checkIfVariableInExitCondList(line2[0], loop.exitCondVars) && !checkIfVariableIterated(line2, 0))
					{
						loop.constantCount = false;
					}
				} while (loopCounter > 0);
				std::cout << "point is:" << loopPointer << "\n";
			}
			else
			{
				loop.constantCount = false;
			}
			//if the loop has no constantCount copy content
			if (!loop.constantCount)
			{
				//reinit
				loopPointer = i;
				loopCounter = 0;
				do
				{
					loop.loopContent.push_back(sourceData[loopPointer]);
					loopPointer++;
					if (sourceData[loopPointer].find("{") != std::string::npos)
					{
						loopCounter++;
					}
					if (sourceData[loopPointer].find("}") != std::string::npos)
					{
						loopCounter--;
					}
				} while (loopCounter > 0);
			}
			loops.push_back(loop);
		}
		else if (line.size() > 0 && line[0].compare("do") == 0)
		{

		}

		/*for (int j = 0; j < line.size(); j++)
		{
			std::cout << line[j] << "\n";
			if (j == 0)
			{
				//loop starts
				if (line[j].compare("while")==0)
				{

				}
			}
		}*/
		line.clear();
	}
	return loops;
}


void CodeParser::fillFunctions(std::vector<std::string> sourceData)
{
	std::vector<std::string> line;
	std::vector<Loopp> loops;
	Funct currentFunct;
	int functCounter = 0;
	int functPointer;
	for (int i = 0; i < sourceData.size(); i++)
	{
		std::istringstream ss(sourceData[i]);
		std::string word; // for storing each word
		//std::cout << sourceData[i] << "\n";
		while (ss >> word)
		{
			line.push_back(word);
		}
		for (int i2 = 0; i2 < this->programFunctions.size(); i2++)
		{
			if (line.size()>1 && programFunctions[i2].functName.compare(line[1]) == 0)
			{
				functPointer = i;
				functCounter = 0;
				do {
					programFunctions[i2].functContent.push_back(sourceData[functPointer]);
					functPointer++;
					if (sourceData[functPointer].find("{") != std::string::npos)
					{
						functCounter++;
					}
					if (sourceData[functPointer].find("}") != std::string::npos)
					{
						functCounter--;
					}
				} while (functCounter > 0);
			}
		}
		line.clear();

	}
}

State CodeParser::getInitialState(FiniteStateMachine finiteStateMachine, std::vector<std::string> sourceCode)
{
	State initialState;
	bool firstAssigFound = false;
	for (int i = 0; i < sourceCode.size() && !firstAssigFound; i++)
	{
		if (sourceCode[i].find(finiteStateMachine.stateVar.name + " = ") != std::string::npos)
		{
			for (int j = 0; j < finiteStateMachine.states.size() && !firstAssigFound; j++)
			{
				if (sourceCode[i].find(finiteStateMachine.states[j].stateName) != std::string::npos)
				{
					initialState = finiteStateMachine.states[j];
					firstAssigFound = true;
				}
			}
		}
	}
	return initialState;
}

std::vector<State> CodeParser::getFinalStates(FiniteStateMachine finiteStateMachine, std::vector<std::string> sourceCode)
{
	std::vector<std::string> remainder;
	State finalState;
	std::vector<State> finalStates;
	/*int i = 0;
	for (int j = 0; j < sourceCode.size(); j++)
	{
		std::cout << sourceCode[j] << "\n";
		remainder.push_back(sourceCode[j]);
	}
	bool firstAssigFound = false;
	for (int i = 0; i < remainder.size() && !firstAssigFound; i++)
	{
		if (remainder[i].find(finiteStateMachine.stateVar.name + " = ") != std::string::npos)
		{
			for (int j = 0; j < finiteStateMachine.states.size() && !firstAssigFound; j++)
			{
				if (remainder[i].find(finiteStateMachine.states[j].stateName) != std::string::npos)
				{
					finalState = finiteStateMachine.states[j];
					firstAssigFound = true;
				}
			}
		}
	}
	if (!firstAssigFound)
	{
		for (int i = 0; i < finiteStateMachine.states.size(); i++)
		{
			finalStates.push_back(finiteStateMachine.states[i]);
		}
	}
	else {
		finalStates.push_back(finalState);
	}*/
	for (int i = 0; i < finiteStateMachine.states.size(); i++)
	{
		finalStates.push_back(finiteStateMachine.states[i]);
	}

	return finalStates;
}

//go through the loop and detect state variables according to alg
std::vector<Variab> CodeParser::obtainStateVariables(Loopp loop)
{
	std::vector<Variab> stateVariables;
	if (!this->programVariables.empty())
	{
		loop.findLoopVariables(this->programVariables);
	}
	if (!loop.loopVars.empty())
	{
		loop.getPossibleStateVars();
	}
	for (int i = 0; i < loop.possibleStateVars.size(); i++)
	{
		//std::cout << "I will check for:" << loop.possibleStateVars[i].name << "\n";
		if (parseAndCheckIfState(loop, loop.possibleStateVars[i]))
		{
			stateVariables.push_back(loop.possibleStateVars[i]);
		}
	}
	return stateVariables;
	/*for (int i = 0; i < loop.possibleStateVars.size(); i++)
	{
		std::cout<<loop.possibleStateVars[i].name << "\n";
	}*/
}

bool CodeParser::parseAndCheckIfState(Loopp loop, Variab variab)
{
	std::vector<std::string> line;
	bool possibleStateVar = false;
	bool isStateVar = false;
	//used to memorize the line in the cond statement when parsing
	int condPoint = 0;
	int delimitCount = 0;
	for (int i = 0; i < loop.loopContent.size(); i++)
	{
		//an if was found
		if (((loop.loopContent[i].find("if(") != std::string::npos) && (loop.loopContent[i].find("else") == std::string::npos))
			|| ((loop.loopContent[i].find("if ") != std::string::npos) && (loop.loopContent[i].find("else") == std::string::npos)))
		{
			condPoint = i;
			bool isFollowedByElse = false;
			std::istringstream ss(loop.loopContent[i]);
			std::string word; // for storing each word
			while (ss >> word)
			{
				line.push_back(word);
				// check if variable used in if
				if (word.compare(variab.name) == 0)
				{
					possibleStateVar = true;
				}
			}
			//has to continue through the if and check if a memory write to the var is done
			//in the loop we check if the if is followed by any else
			do {
				//std::cout << "I do a round\n";
				if (possibleStateVar)
				{
					delimitCount = 0;
					if (loop.loopContent[condPoint].find("{") != std::string::npos)
					{
						delimitCount++;
					}
					//go through the cond statement
					do
					{
						condPoint++;
						if ((loop.loopContent[condPoint].find(variab.name + "= ") != std::string::npos)
							|| (loop.loopContent[condPoint].find(variab.name + " = ") != std::string::npos)
							|| (loop.loopContent[condPoint].find(variab.name + "+=") != std::string::npos)
							|| (loop.loopContent[condPoint].find(variab.name + "-=") != std::string::npos)
							|| (loop.loopContent[condPoint].find(variab.name + "++") != std::string::npos)
							|| (loop.loopContent[condPoint].find(variab.name + "--") != std::string::npos))
						{
							//std::cout << "Var just became " << variab.name << "\n";
							isStateVar = true;
						}
						if (loop.loopContent[condPoint].find("{") != std::string::npos)
						{
							delimitCount++;
						}
						if (loop.loopContent[condPoint].find("}") != std::string::npos)
						{
							delimitCount--;
						}
						//std::cout << "Counter is:" << loop.loopContent[condPoint]<<"   "<<delimitCount << "\n";
					} while ((delimitCount > 0) && (!isStateVar));
					//std::cout << "I got to:" << loop.loopContent[condPoint+1] << "\n";
					if ((loop.loopContent[condPoint].find("else") != std::string::npos) || (loop.loopContent[condPoint + 1].find("else") != std::string::npos))
					{
						//std::cout << "agaaain???\n";
						isFollowedByElse = true;
					}
					else
					{
						isFollowedByElse = false;
					}
				}
			} while (isFollowedByElse);
		}
		else if ((loop.loopContent[i].find("switch(") != std::string::npos) || (loop.loopContent[i].find("switch ") != std::string::npos))
		{
			//std::cout << "found a switch\n";
			std::istringstream ss(loop.loopContent[i]);
			std::string word; // for storing each word
			condPoint = i;
			while (ss >> word)
			{
				line.push_back(word);
				// check if variable used in if
				if (word.compare(variab.name) == 0)
				{
					possibleStateVar = true;
				}
			}
			if (possibleStateVar)
			{
				//std::cout << "found one\n";
				delimitCount = 0;
				if (loop.loopContent[condPoint].find("{") != std::string::npos)
				{
					delimitCount++;
				}
				//go through the cond statement
				do
				{
					condPoint++;
					if ((loop.loopContent[condPoint].find(variab.name + "= ") != std::string::npos)
						|| (loop.loopContent[condPoint].find(variab.name + " = ") != std::string::npos)
						|| (loop.loopContent[condPoint].find(variab.name + "+=") != std::string::npos)
						|| (loop.loopContent[condPoint].find(variab.name + "-=") != std::string::npos)
						|| (loop.loopContent[condPoint].find(variab.name + "++") != std::string::npos)
						|| (loop.loopContent[condPoint].find(variab.name + "--") != std::string::npos))
					{
						isStateVar = true;
					}
					if (loop.loopContent[condPoint].find("{") != std::string::npos)
					{
						delimitCount++;
					}
					if (loop.loopContent[condPoint].find("}") != std::string::npos)
					{
						delimitCount--;
					}
					//std::cout << "Counter is:" << loop.loopContent[condPoint] << "   " << delimitCount << "\n";
				} while ((delimitCount > 0) && (!isStateVar));
			}
		}
		line.clear();
	}
	return isStateVar;
}

std::vector<State> CodeParser::obtainStates(Loopp loop, Variab stateVar)
{
	std::vector<State> states;
	Enum enumeration;

	if (stateVar.isEnum)
	{
		for (int i = 0; i < this->enums.size(); i++)
		{
			std::cout << "\n" << stateVar.type << "\n";
			if (this->enums[i].enumName.compare(stateVar.type) == 0)
			{
				enumeration = this->enums[i];
				std::cout << "\n" << enumeration.enumName << "\n";
			}
		}
		for (int j = 0; j < enumeration.enumElements.size(); j++)
		{
			State state;
			state.stateName = enumeration.enumElements[j].enumElName;
			states.push_back(state);
		}
		std::cout << "\nEnum states\n";
		for (int x = 0; x < states.size(); x++)
		{
			std::cout << states[x].stateName << "\n";
		}
	}
	//can only be int otherwise 
	else
	{
		//to find the states we have to check all the values assigned to the variable inside the loop
		Funct funct = loop.functionContainingLoop;
		std::vector<std::string> line;
		for (int i = 0; i < funct.functContent.size(); i++)
		{
			if (funct.functContent[i].find(stateVar.name) != std::string::npos)
			{
				std::istringstream ss(loop.loopContent[i]);
				std::string word; // for storing each word
				while (ss >> word)
				{
					line.push_back(word);
					// check if variable used in if

				}
				for (int j = 0; j < line.size(); j++)
				{
					if ((line[j].compare(stateVar.name) == 0) && (line[j + 1].compare("=") == 0))
					{
						State state;
						state.stateName = line[j];
						states.push_back(state);
					}
				}
			}
		}
	}
	return states;
}


//int identifyReductionVariables

//main big parsing function
std::vector<FiniteStateMachine> CodeParser::parseCode(std::vector<std::string> sourceData)
{
	std::vector<Loopp> loops;
	std::vector<Variab> stateVars;
	std::vector<FiniteStateMachine> finiteStateMachines;
	getVariabs(sourceData);

	fillFunctions(sourceData);
	loops = findLoops(sourceData);
	for (int i = 0; i < loops.size(); i++)
	{
		//obtain the state variables
		stateVars = obtainStateVariables(loops[i]);


		//based on the obtained state vars create the fsms (each fsm has a state var)
		for (int j = 0; j < stateVars.size(); j++)
		{
			//std::cout << stateVars[j].name << " ";
			FiniteStateMachine fsm;
			fsm.name = " FSM ";
			fsm.stateVar = stateVars[i];
			fsm.loop = loops[i];
			for (int l = 0; l < fsm.loop.loopContent.size(); l++)
			{
				std::cout << fsm.loop.loopContent[l] << "\n";
			}
			fsm.states = obtainStates(loops[i], stateVars[i]);
			finiteStateMachines.push_back(fsm);
		}
	}


	for (int i = 0; i < finiteStateMachines.size(); i++)
	{
		std::vector<TransitionFunction> transitionFunctions;
		State initialState;
		State finalState;

		transitionExtractor.programVariables = programVariables;
		transitionExtractor.enums = enums;
		transitionFunctions = transitionExtractor.extractTransitions(sourceData, finiteStateMachines[i]);
		finiteStateMachines[i].transitionFunctions = transitionFunctions;

		initialState = getInitialState(finiteStateMachines[i], sourceData);
		finiteStateMachines[i].initialState = initialState;

		std::vector<State> finalStates = getFinalStates(finiteStateMachines[i], sourceData);
		finiteStateMachines[i].finalStates = finalStates;
	}

	return finiteStateMachines;
}