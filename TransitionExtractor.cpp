#include "TransitionExtractor.h"


TransitionExtractor::TransitionExtractor()
{
}

//function that verifies if assignments are inside or outside the loop
void TransitionExtractor::checkIfAssigsInsideLoop(std::vector<std::string> sourceData, Loopp loop)
{
	//std::size_t pos = sourceData.find()
	//std::string remainder  = 
	std::vector<std::string> remainderBefore;
	std::vector<std::string> remainderAfter;
	remainderBefore = getCodeBeforeLoop(sourceData, loop);
	remainderAfter = getCodeAfterLoop(sourceData, loop);

	for (int k = 0; k < remainderBefore.size(); k++)
	{
		for (int k1 = 0; k1 < assignments.size(); k1++)
		{
			//std::cout << " aaaa " << assignments[k1].assigText << " \n ";
			if ((remainderBefore[k].find(assignments[k1].assigText) != std::string::npos) && assignments[k1].insideLoop)
			{
				assignments[k1].insideLoop = false;
				//std::cout << "\n\n\n Get out " << assignments[k1].assigText << "\n\n\n";
			}
		}
	}

	for (int k = 0; k < remainderAfter.size(); k++)
	{
		for (int k1 = 0; k1 < assignments.size(); k1++)
		{
			//std::cout << " aaaa " << assignments[k1].assigText << " \n ";
			if ((remainderAfter[k].find(assignments[k1].assigText) != std::string::npos) && assignments[k1].insideLoop)
			{
				assignments[k1].insideLoop = false;
				//std::cout << "\n\n\n Get out " << assignments[k1].assigText << "\n\n\n";
			}
		}
	}
}


std::vector<Constraint> TransitionExtractor::extractFromIf(std::vector<std::string> dataContent, Assig assig1, Assig assig2, int loopPoint)
{
	//std::cout << "Found if!!\n";
	bool assig2Found = false;
	int delimitCount;
	int ifCondPoint;
	std::string tempCond;
	std::size_t pos;
	std::size_t pos1;
	ifCondPoint = loopPoint;
	assig2Found = false;
	bool isFollowedByElseIf = false;
	bool isFollowedByElse = false;
	std::vector<std::string> ifConditions;

	std::vector<Constraint> constraints;
	//has to continue through the if and check if a memory write to the var is done
	//in the loop we check if the if is followed by any else
	do {
		//std::cout << "LOOOp\n";
		pos = dataContent[ifCondPoint].find("(");
		pos1 = dataContent[ifCondPoint].find(")");
		tempCond = dataContent[ifCondPoint].substr(pos + 1, pos1 - pos - 1);
		//std::cout << "TEMP COND IF IS" << tempCond << "\n";
		//std::cout << "I do a round\n";
		delimitCount = 0;
		if (dataContent[ifCondPoint].find("{") != std::string::npos)
		{
			delimitCount++;
		}
		//go through the cond statement
		do
		{
			ifCondPoint++;
			if (dataContent[ifCondPoint].find("{") != std::string::npos)
			{
				delimitCount++;
			}
			if (dataContent[ifCondPoint].find("}") != std::string::npos)
			{
				delimitCount--;
			}
			if (dataContent[ifCondPoint].find(assig2.assigText) != std::string::npos)
			{
				//std::cout << "\nFOUND ASSIG " << dataContent[ifCondPoint] << "\n";
				assig2Found = true;
			}
			//std::cout << "Counter is:" << loop.loopContent[condPoint]<<"   "<<delimitCount << "\n";
		} while ((delimitCount > 0) && (!assig2Found));
		if (assig2Found)
		{
			Constraint ifConstraint;
			ifConstraint.constraintString = tempCond;
			constraints.push_back(ifConstraint);
		}
		else
		{
			//add cond to vector so in case of else it can be found
			ifConditions.push_back(tempCond);
			if ((dataContent[ifCondPoint].find("else if") != std::string::npos) || (dataContent[ifCondPoint + 1].find("else if") != std::string::npos))
			{
				//increase cond point so it goes well on else
				if (dataContent[ifCondPoint + 1].find("else if") != std::string::npos)
				{
					ifCondPoint++;
				}
				//std::cout << "agaaain???\n";
				isFollowedByElseIf = true;
			}
			else
			{
				isFollowedByElseIf = false;
				if ((dataContent[ifCondPoint].find("else") != std::string::npos) || (dataContent[ifCondPoint + 1].find("else") != std::string::npos))
				{
					if (dataContent[ifCondPoint + 1].find("else") != std::string::npos)
					{
						ifCondPoint++;
					}
					isFollowedByElse = true;
				}
			}
		}
		//std::cout << "I got to:" << loop.loopContent[condPoint+1] << "\n";
	} while (isFollowedByElseIf && !assig2Found);
	if (isFollowedByElse && !assig2Found)
	{
		//std::cout << "\nINSIDE ELSE";
		delimitCount = 0;
		if (dataContent[ifCondPoint].find("{") != std::string::npos)
		{
			delimitCount++;
		}
		// if is else then negate all the stuff gathered from ifs and elseifs
		do
		{
			ifCondPoint++;
			if (dataContent[ifCondPoint].find("{") != std::string::npos)
			{
				delimitCount++;
			}
			if (dataContent[ifCondPoint].find("}") != std::string::npos)
			{
				delimitCount--;
			}
			if (dataContent[ifCondPoint].find(assig2.assigText))
			{
				assig2Found = true;
			}
			//std::cout << "Counter is:" << loop.loopContent[condPoint]<<"   "<<delimitCount << "\n";
		} while ((delimitCount > 0) && (!assig2Found));
		if (assig2Found)
		{
			// add all the negated conds from the temp
			for (int j = 0; j < ifConditions.size(); j++)
			{
				Constraint constraint;
				constraint.constraintString = "! " + ifConditions[j];
				constraints.push_back(constraint);
			}
		}
	}
	return constraints;
}

std::vector<Constraint> TransitionExtractor::extractFromSwitch(std::vector<std::string> dataContent, Assig assig1, Assig assig2, int loopPoint)
{
	int delimitCount;
	int switchCondPoint;
	std::string tempCond;
	bool assig2Found = false;
	std::vector<Constraint> constraints;
	//std::cout << "SW loop \n";
	delimitCount = 0;
	std::string switchVar;
	bool assigFoundInDefault = false;
	//count the conditions aka the cases
	int condCount = 0;
	std::vector<std::string> switchConditions;
	switchCondPoint = loopPoint;
	//switchVar =
	size_t pos = dataContent[switchCondPoint].find("(");
	size_t pos1 = dataContent[switchCondPoint].find(")");
	switchVar = dataContent[switchCondPoint].substr(pos + 1, pos1 - pos - 2);
	//std::cout << "SW VAR " << switchVar << "\n";
	if (dataContent[switchCondPoint].find("{") != std::string::npos)
	{
		delimitCount++;
	}
	//parse through the switch case
	do
	{
		//std::cout << "SWP: " << switchCondPoint << "\n";
		switchCondPoint++;
		if (dataContent[switchCondPoint].find("{") != std::string::npos)
		{
			delimitCount++;
		}
		if (dataContent[switchCondPoint].find("}") != std::string::npos)
		{
			delimitCount--;
		}
		//if get to a case save the condition
		if (dataContent[switchCondPoint].find("case") != std::string::npos)
		{

			size_t pos3 = dataContent[switchCondPoint].find(" ");
			size_t pos4 = dataContent[switchCondPoint].find(":");
			tempCond = dataContent[switchCondPoint].substr(pos3 + 1, pos4 - pos3 - 1);
			//std::cout << "SWP: " << switchCondPoint << "\n";
			//std::cout << "TEMP COND SW IS " << tempCond << "\n";
			condCount++;
			switchConditions.push_back(tempCond);
		}
		//found default
		if (dataContent[switchCondPoint].find("default") != std::string::npos)
		{
			//std::cout << "DEF  SWP: " << switchCondPoint << "\n";
			// in the case of default we have to search separately because if found the assignment, then the cond are taken all from
			// the vector and negated
			int delimitCountLoop = 1;
			std::vector<std::string> defaultSwitchConditions;
			int switchDefaultCondPoint = switchCondPoint;
			do {
				//std::cout << "DO A default round\n";
				switchDefaultCondPoint++;
				if (dataContent[switchDefaultCondPoint].find("{") != std::string::npos)
				{
					delimitCountLoop++;
				}
				if (dataContent[switchDefaultCondPoint].find("}") != std::string::npos)
				{
					delimitCountLoop--;
				}
				if (dataContent[switchDefaultCondPoint].find(assig2.assigText) != std::string::npos)
				{
					//std::cout << "\nFound!!";
					assig2Found = true;
					for (int k = 0; k < switchConditions.size(); k++)
					{
						Constraint constraint;
						constraint.constraintString = switchVar + " != " + switchConditions[k];
						constraints.push_back(constraint);
					}
				}
			} while ((delimitCountLoop > 0) && (!assig2Found));
			switchCondPoint = switchDefaultCondPoint;
		}
		if (!assig2Found && (dataContent[switchCondPoint].find(assig2.assigText) != std::string::npos))
		{
			assig2Found = true;
			std::string trueCond = switchConditions[switchConditions.size() - 1];
			Constraint constraint;
			constraint.constraintString = switchVar + " == " + trueCond;
			constraints.push_back(constraint);
		}
		//std::cout << "Counter is:" << loop.loopContent[condPoint]<<"   "<<delimitCount << "\n";
	} while ((delimitCount > 0) && (!assig2Found));
	return constraints;
}


// adds as a constraint the initial value of the state variable
std::vector<Constraint> TransitionExtractor::addInitialConstraint(std::vector<Constraint> constraints, Assig assig1)
{
	Constraint constraint;
	constraint.constraintString = assig1.stateVar.name + " " + EQ + " " + assig1.assignedState.stateName;
	constraint.elem1 = assig1.stateVar.name;
	constraint.op = EQ;
	constraint.elem2 = assig1.assignedState.stateName;
	std::cout << " \n Initial constraint is: " << constraint.constraintString << "\n";

	std::vector<Constraint> finalConstraints;
	for (int i = 0; i < constraints.size(); i++)
	{
		finalConstraints.push_back(constraints[i]);
	}
	finalConstraints.push_back(constraint);

	return finalConstraints;
}

//function that retrieves the constraints
std::vector<Constraint> TransitionExtractor::getConstraints(std::vector<std::string> sourceData, Loopp loop, Assig assig1, Assig assig2, bool& noPathPossible)
{
	bool inLoop = false;
	bool foundAssig1 = false;
	bool foundAssig2 = false;
	std::vector<Constraint> constraints;
	std::vector<std::string> restOfCodeBefore = getCodeBeforeLoop(sourceData, loop);
	std::vector<std::string> restOfCodeAfter = getCodeAfterLoop(sourceData, loop);
	std::vector<std::string> restOfCode;
	for (int i = 0; i < restOfCodeBefore.size(); i++)
	{
		restOfCode.push_back(restOfCodeBefore[i]);
	}
	for (int i = 0; i < restOfCodeAfter.size(); i++)
	{
		restOfCode.push_back(restOfCodeAfter[i]);
	}
	for (int i = 0; i < sourceData.size(); i++)
	{
		std::istringstream ss(sourceData[i]);
		std::string word; // for storing each word
		std::vector<std::string> line;
		while (ss >> word)
		{
			line.push_back(word);
		}
	}
	//case 1 outside of loop

	if ((!assig1.insideLoop) && (!assig2.insideLoop))
	{
		std::cout << "\nCASE 1";
		bool assig1Found = false;
		bool assig2Found = false;
		int assig1Line = 0;
		int assig2Line = 0;
		// no var is inside loop so look at the rest of code
		for (int i = 0; i < restOfCode.size() && !assig1Found; i++)
		{
			if (restOfCode[i].compare(assig1.assigText) == 0)
			{
				foundAssig1 = true;
				assig1Line = i;
			}
			if (restOfCode[i].compare(assig2.assigText) == 0)
			{
				foundAssig2 = true;
				assig2Line = i;
			}
		}
		if (assig1Line < assig2Line)
		{
			for (int j = assig1Line; j < assig2Line + 1; j++)
			{
				if (((restOfCode[j].find("if(") != std::string::npos) && (restOfCode[j].find("else") == std::string::npos))
					|| ((restOfCode[j].find("if ") != std::string::npos) && (restOfCode[j].find("else") == std::string::npos)))
				{
					std::vector<Constraint> ifConstraints = extractFromIf(restOfCode, assig1, assig2, j);
					for (int j = 0; j < ifConstraints.size(); j++)
					{
						constraints.push_back(ifConstraints[j]);
					}
				}
				if (restOfCode[j].find("switch") != std::string::npos)
				{
					std::vector<Constraint> switchConstraints = extractFromSwitch(restOfCode, assig1, assig2, j);
					for (int j = 0; j < switchConstraints.size(); j++)
					{
						constraints.push_back(switchConstraints[j]);
					}
				}
			}
		}
	}

	//case 2 first inside of loop, second outside of loop

	else if ((assig1.insideLoop) && (!assig2.insideLoop))
	{
		std::cout << "\nCASE 2";
		std::cout << "\nExit cond statement is: " << loop.exitCondStatement << "\n";
		std::string loopCond = convertCondition(loop.exitCondStatement);
		Constraint loopConstraint;

		//in order to keep track of the assig1 finding
		bool assig2FoundAfterLoop = false;
		int assig2FoundAfterLoopLine;

		loopConstraint.constraintString = loopCond;
		constraints.push_back(loopConstraint);
		std::cout << "Exit cond after change is: " << loopCond << "\n";

		// negate the loop conditions
		/*std::istringstream ss(loop.exitCondStatement);
		std::string word; // for storing each word
		std::vector<std::string> line;
		bool varFound = false;
		bool condChanged = false;

		while (ss >> word)
		{
			line.push_back(word);
		}*/

		Constraint constraint;
		constraint.constraintString = "! " + loop.exitCondStatement;
		constraints.push_back(constraint);


		// first check the remainder code and see if state is before or after loop
		for (int i = 0; i < restOfCodeAfter.size(); i++)
		{
			if (restOfCodeAfter[i].find(assig2.assigText) != std::string::npos)
			{
				std::cout << "\n Assig2 is: " << assig2.assigText << "\n";
				assig2FoundAfterLoop = true;
				assig2FoundAfterLoopLine = i;
			}
		}

		//if is after, traverse the code and check for ifs and switches 
		if (assig2FoundAfterLoop)
		{
			for (int j = 0; j < assig2FoundAfterLoopLine; j++)
			{
				if (((restOfCodeAfter[j].find("if(") != std::string::npos) && (restOfCodeAfter[j].find("else") == std::string::npos))
					|| ((restOfCodeAfter[j].find("if ") != std::string::npos) && (restOfCodeAfter[j].find("else") == std::string::npos)))
				{
					std::vector<Constraint> ifConstraints = extractFromIf(restOfCodeAfter, assig1, assig2, j);
					for (int k = 0; k < ifConstraints.size(); k++)
					{
						constraints.push_back(ifConstraints[k]);
					}
				}
				if (restOfCodeAfter[j].find("switch") != std::string::npos)
				{
					std::vector<Constraint> switchConstraints = extractFromSwitch(restOfCodeAfter, assig1, assig2, j);
					for (int k = 0; k < switchConstraints.size(); k++)
					{
						constraints.push_back(switchConstraints[k]);
					}
				}
			}
		}
		else
		{
			noPathPossible = true;
		}
	}

	//case 3 first outside of loop, second inside of loop

	else if ((!assig1.insideLoop) && (assig2.insideLoop))
	{
		std::cout << "\nCASE 3";
		//std::cout << "\nAssig 1 is: " << assig1.assigText << " Assig 2 is: " << assig2.assigText << "\n";
		std::cout << "\nExit cond statement is: " << loop.exitCondStatement << "\n";
		std::string loopCond = convertCondition(loop.exitCondStatement);
		Constraint loopConstraint;

		//in order to keep track of the assig1 finding
		bool assig1FoundBeforeLoop;
		int assig1FoundBeforeLoopLine;

		loopConstraint.constraintString = loopCond;
		constraints.push_back(loopConstraint);
		//std::cout << "Exit cond after change is: " << loopCond << "\n";
		//get the rest content

		// first check the remainder code and see if state is before or after loop
		for (int i = 0; i < restOfCodeBefore.size(); i++)
		{
			if (restOfCodeBefore[i].find(assig1.assigText))
			{
				assig1FoundBeforeLoop = true;
				assig1FoundBeforeLoopLine = i;
			}
		}
		if (assig1FoundBeforeLoop)
		{
			for (int j = assig1FoundBeforeLoopLine; j < restOfCodeBefore.size(); j++)
			{
				if (((restOfCodeBefore[j].find("if(") != std::string::npos) && (restOfCodeBefore[j].find("else") == std::string::npos))
					|| ((restOfCodeBefore[j].find("if ") != std::string::npos) && (restOfCodeBefore[j].find("else") == std::string::npos)))
				{
					std::vector<Constraint> ifConstraints = extractFromIf(restOfCodeBefore, assig1, assig2, j);
					for (int k = 0; k < ifConstraints.size(); k++)
					{
						constraints.push_back(ifConstraints[k]);
					}
				}
				if (restOfCodeBefore[j].find("switch") != std::string::npos)
				{
					std::vector<Constraint> switchConstraints = extractFromSwitch(restOfCodeBefore, assig1, assig2, j);
					for (int k = 0; k < switchConstraints.size(); k++)
					{
						constraints.push_back(switchConstraints[k]);
					}
				}
			}
		}
		else
		{
			std::cout << "\nNot before the loop so it doesnt matter";
		}
		// look inside the loop code
		for (int i = 0; i < loop.loopContent.size(); i++)
		{
			if (((loop.loopContent[i].find("if(") != std::string::npos) && (loop.loopContent[i].find("else") == std::string::npos))
				|| ((loop.loopContent[i].find("if ") != std::string::npos) && (loop.loopContent[i].find("else") == std::string::npos)))
			{
				std::vector<Constraint> ifConstraints = extractFromIf(loop.loopContent, assig1, assig2, i);
				for (int j = 0; j < ifConstraints.size(); j++)
				{
					constraints.push_back(ifConstraints[j]);
				}
			}
			if (loop.loopContent[i].find("switch") != std::string::npos)
			{
				std::vector<Constraint> switchConstraints = extractFromSwitch(loop.loopContent, assig1, assig2, i);
				for (int j = 0; j < switchConstraints.size(); j++)
				{
					constraints.push_back(switchConstraints[j]);
				}
			}
		}
	}

	//case 4 both inside

	else
	{
		std::cout << "\nCASE 4\n";
		// take the loop conditions as constraints

		//std::cout << "\nAssig 1 is: " << assig1.assigText << " Assig 2 is: " << assig2.assigText << "\n";
		std::string loopCond = convertCondition(loop.exitCondStatement);
		Constraint loopConstraint;

		loopConstraint.constraintString = loopCond;
		constraints.push_back(loopConstraint);

		for (int i = 0; i < loop.loopContent.size(); i++)
		{
			if (((loop.loopContent[i].find("if(") != std::string::npos) && (loop.loopContent[i].find("else") == std::string::npos))
				|| ((loop.loopContent[i].find("if ") != std::string::npos) && (loop.loopContent[i].find("else") == std::string::npos)))
			{
				std::vector<Constraint> ifConstraints = extractFromIf(loop.loopContent, assig1, assig2, i);
				for (int j = 0; j < ifConstraints.size(); j++)
				{
					constraints.push_back(ifConstraints[j]);
				}
			}
			if (loop.loopContent[i].find("switch") != std::string::npos)
			{
				std::vector<Constraint> switchConstraints = extractFromSwitch(loop.loopContent, assig1, assig2, i);
				for (int j = 0; j < switchConstraints.size(); j++)
				{
					constraints.push_back(switchConstraints[j]);
				}
			}
		}
	}
	/*
		for (int i = 0; i < constraints.size(); i++)
		{
			std::cout << constraints[i].constraintString << " ";
		}
		*/
	return constraints;
}

//function to extract the source code that does not contain the loop
std::vector<std::string> TransitionExtractor::getCodeBeforeLoop(std::vector<std::string> sourceData, Loopp loop)
{
	std::vector<std::string> remainder;
	bool foundBeginning = false;
	int i = 0;

	//get the rest content
	while (!foundBeginning)
	{
		remainder.push_back(sourceData[i]);
		//found the first line of loop
		if (sourceData[i + 1].find(loop.loopContent[0]) != std::string::npos)
		{
			foundBeginning = true;
		}
		i++;
	}
	return remainder;
}

//function to extract the source code that does not contain the loop
std::vector<std::string> TransitionExtractor::getCodeAfterLoop(std::vector<std::string> sourceData, Loopp loop)
{
	std::vector<std::string> remainder;
	int i = 0;
	for (int j = i + loop.loopContent.size() + 1; j < sourceData.size(); j++)
	{
		remainder.push_back(sourceData[j]);
	}
	return remainder;
}

//converts the condition elements with != Null or == Null
std::string TransitionExtractor::convertCondition(std::string condition)
{
	std::string newCondition;
	std::string newFinalCond;
	std::istringstream ss(condition);
	std::string word; // for storing each word
	std::vector<std::string> line;
	bool varFound = false;
	bool condChanged = false;

	//std::cout << "COND is:  " << condition << "\n";
	while (ss >> word)
	{
		line.push_back(word);
	}
	for (int i = 0; i < line.size() - 1; i++)
	{
		//std::cout << "\nline is " << line[i] << "\n";
		for (int j = 0; j < programVariables.size() && !varFound; j++)
		{
			//found a variable
			if (line[i].compare(programVariables[j].name) == 0)
			{
				varFound = true;
				//check if it is followed by comp
				if ((line[i + 1].find("=") == std::string::npos) && (line[i + 1].find(">") == std::string::npos) && (line[i + 1].find("<") == std::string::npos))
				{
					//std::cout << "\nVar found is: " << programVariables[j].name << "\n";
					condChanged = true;
					//is null
					if ((i > 0) && (line[i - 1].compare("!") == 0))
					{
						newCondition = line[i] + " == " + NULL_DATA;
					}
					//is not null
					else
					{
						newCondition = line[i] + " != " + NULL_DATA;
					}
					//std::cout << "\nVar found is: " << programVariables[j].name << " and new cond is" << newCondition <<"\n";
				}
			}
		}
		//std::cout << " and new cond is" << newCondition << "\n";
		if (condChanged)
		{
			newFinalCond = newFinalCond + " " + newCondition;
		}
		else
		{
			newFinalCond = newFinalCond + " " + line[i];
		}
		varFound = false;
		condChanged = false;
		//std::cout << "\n final cond: " << newFinalCond << "\n";
	}
	newFinalCond = newFinalCond + " " + line[line.size() - 1];
	return newFinalCond;
}

std::vector<TransitionFunction> TransitionExtractor::extractTransitions(std::vector<std::string> sourceData, FiniteStateMachine finiteStateMachine)
{

	std::vector<TransitionFunction> transitionFunctions;
	std::vector<Constraint> constraints;
	bool noPathPossible = false;

	//generate Assigs
	for (int i = 0; i < finiteStateMachine.states.size(); i++)
	{
		Assig assig;
		assig.stateVar = finiteStateMachine.stateVar;
		assig.assignedState = finiteStateMachine.states[i];
		assig.assigText = assig.stateVar.name + " = " + assig.assignedState.stateName;
		assignments.push_back(assig);
	}

	//just to see the programvariables
	for (int i = 0; i < programVariables.size(); i++)
	{
		std::cout << "\n Prog var: " << programVariables[i].name << " has type " << programVariables[i].type;
	}

	//get the functionContent and establish where the assignments are because there are several cases
	std::vector<std::string> functionContent = finiteStateMachine.loop.functionContainingLoop.functContent;
	checkIfAssigsInsideLoop(functionContent, finiteStateMachine.loop);

	//traverse the assignments and create the paths
	for (int j = 0; j < assignments.size(); j++)
	{
		for (int j1 = 0; j1 < assignments.size(); j1++)
		{
			noPathPossible = false;
			Path path;
			path.assig1 = assignments[j];
			path.assig2 = assignments[j1];
			constraints = getConstraints(functionContent, finiteStateMachine.loop, path.assig1, path.assig2, noPathPossible);

			if (!noPathPossible)
			{
				//update path constraints
				path.pathConstraints = addInitialConstraint(constraints, path.assig1);

				constraintSolver.programVariables = programVariables;
				constraintSolver.enums = enums;
				//check for conflict with the constraint solver and if none the transition function is obtained
				if (!constraintSolver.areConflicts(path))
				{
					TransitionFunction transitionFunction;
					transitionFunction.startState = path.assig1.assignedState;
					transitionFunction.finishState = path.assig2.assignedState;
					transitionFunctions.push_back(transitionFunction);
				}
				else
				{
					//std::cout << "\n Conflicts have been found!! \n";
					//if no conflicts it means there is a transition function
					//Transition
				}
			}
			else
			{
				std::cout << " \n NO PATH POSSIBLE \n";
			}
		}
	}

	return transitionFunctions;
}