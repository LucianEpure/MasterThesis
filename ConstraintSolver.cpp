#include "ConstraintSolver.h"

ConstraintSolver::ConstraintSolver()
{
}

//function that checks if there are conflicts between the conditions of the path from assig1 to assig2

bool ConstraintSolver::areConflicts(Path path)
{
	std::vector<Constraint> constraints;
	bool constraintsConflict = false;
	constraints = convertConstraints(path.pathConstraints);
	/*std::cout << "\n";
	for (int i = 0; i < constraints.size(); i++)
	{
		std::cout << constraints[i].constraintString << "\n";
	}*/

	for (int i = 0; i < constraints.size(); i++)
	{
		for (int j = 0; j < constraints.size(); j++)
		{
			if (i != j)
			{
				//check if in the second constraint there is any of the elements from the first constraint that are also program variables
				if (
					((checkIfProgramVariable(constraints[i].elem1)) && (constraints[j].constraintString.find(constraints[i].elem1) != std::string::npos))
					||
					((checkIfProgramVariable(constraints[i].elem2)) && (constraints[j].constraintString.find(constraints[i].elem2) != std::string::npos))
					)
				{
					constraintsConflict = checkIfContradict(constraints[i], constraints[j]);
					//std::cout << "\n ANd the constraints CONFLICT!! " << constraintsConflict << " \n";
					if (constraintsConflict)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}


//from the constraint string fills all the elements.
std::vector<Constraint> ConstraintSolver::parseAndArrangeConstraints(std::vector<Constraint> pathConstraints)
{
	std::vector<Constraint> parsedConstraints;
	std::vector<std::string> line;
	for (int i = 0; i < pathConstraints.size(); i++)
	{
		line.clear();
		Constraint constraint;
		std::istringstream ss(pathConstraints[i].constraintString);
		constraint.constraintString = pathConstraints[i].constraintString;
		std::string word; // for storing each word
		while (ss >> word)
		{
			line.push_back(word);
		}
		if (line.size() == 3)
		{
			//std::cout << "Hello\n";
			constraint.elem1 = line[0];
			constraint.op = line[1];
			constraint.elem2 = line[2];
		}
		parsedConstraints.push_back(constraint);
	}
	return parsedConstraints;
}

//function that creates a vector in which each element is one single constraint (no && separates them anymore)
std::vector<Constraint> ConstraintSolver::convertConstraints(std::vector<Constraint> pathConstraints)
{
	std::vector<Constraint> obtainedConstraints;
	std::vector<Constraint> parsedConstraints;
	for (int i = 0; i < pathConstraints.size(); i++)
	{
		//found &&
		if (pathConstraints[i].constraintString.find(AND_OP) != std::string::npos)
		{
			//remove paranthesis
			pathConstraints[i].constraintString.erase(std::remove(pathConstraints[i].constraintString.begin(), pathConstraints[i].constraintString.end(), LEFT_PAR_CHAR), pathConstraints[i].constraintString.end());
			pathConstraints[i].constraintString.erase(std::remove(pathConstraints[i].constraintString.begin(), pathConstraints[i].constraintString.end(), RIGHT_PAR_CHAR), pathConstraints[i].constraintString.end());

			int start = 0;
			int end = pathConstraints[i].constraintString.find(AND_OP);
			//parse after &&
			while (end != -1) {
				Constraint constraint;
				constraint.constraintString = pathConstraints[i].constraintString.substr(start, end - start);
				obtainedConstraints.push_back(constraint);
				start = end + 2;
				end = pathConstraints[i].constraintString.find(AND_OP, start);
			}
			Constraint constraint1;
			constraint1.constraintString = pathConstraints[i].constraintString.substr(start, end - start);
			obtainedConstraints.push_back(constraint1);
		}
		else
		{
			obtainedConstraints.push_back(pathConstraints[i]);
		}
	}
	parsedConstraints = parseAndArrangeConstraints(obtainedConstraints);
	return parsedConstraints;
}

bool ConstraintSolver::checkIfContradict(Constraint constraint1, Constraint constraint2)
{
	//std::cout << "Constraints to contradict are: " << constraint1.constraintString << " and " << constraint2.constraintString;
	//std::cout << "\n El 1 is " << constraint1.elem1;
	//std::cout << "\n";
	Variab foundVariab;
	std::string varType;
	bool constraint1CompToNull = false;
	bool constraint2CompToNull = false;
	bool typeFound = false;
	for (int i = 0; i < programVariables.size(); i++)
	{
		//std::cout << "Prog var is: " << programVariables[i].name << " \n";
		//check if there is a program variable in the constraint
		if ((constraint1.constraintString.find(programVariables[i].name) != std::string::npos) && (constraint2.constraintString.find(programVariables[i].name) != std::string::npos))
		{
			//std::cout << "And the var is: " << programVariables[i].name << "\n";

			//check if the constraint is compared to null value
			if (constraint1.constraintString.find(NULL_DATA) != std::string::npos)
			{
				constraint1CompToNull = true;
			}
			if (constraint2.constraintString.find(NULL_DATA) != std::string::npos)
			{
				constraint2CompToNull = true;
			}

			//the variable is in both constraint elements so we do the complex checking
			if (checkIfProgramVariable(constraint1.elem1) && checkIfProgramVariable(constraint1.elem2))
			{
				//std::cout << "\n Both";
			}
			//the variable is just in first element of the first constraint
			else if (checkIfProgramVariable(constraint1.elem1) && !checkIfProgramVariable(constraint1.elem2))
			{
				//std::cout << "\n First";
				for (int j = 0; j < programVariables.size() && !typeFound; j++)
				{
					if (programVariables[j].name.compare(constraint1.elem1) == 0)
					{
						typeFound = true;
						foundVariab = programVariables[j];
						//std::cout << "\n Type of " << programVariables[j].name<<" is: " << foundVariab.type<<"\n\n\n\n\n";
					}
				}
				if (foundVariab.isEnum)
				{
					//std::cout << " \n We have an enumeratioooon \n";
					//for now if there are just elements of an enumeration we check only if equal or different
					if ((constraint1.op.compare(EQ) == 0) && (constraint2.op.compare(EQ) == 0))
					{
						//std::cout << "\n ENum EQ and EQ \n";
						if (constraint1.elem2.compare(constraint2.elem2) == 0)
						{
							return false;
						}
						else
							return true;
					}
					else if ((constraint1.op.compare(EQ) == 0) && (constraint2.op.compare(DIFF) == 0))
					{
						//std::cout << "\n ENum EQ and DIFF \n";
						if (constraint1.elem2.compare(constraint2.elem2) == 0)
						{
							return true;
						}
						else
							return false;
					}
					else if ((constraint1.op.compare(DIFF) == 0) && (constraint2.op.compare(DIFF) == 0))
					{
						//std::cout << "\n ENum DIFF and DIFF \n";
						for (int k = 0; k < enums.size(); k++)
						{
							//find the enumeration and check that it has more than 2 elements
							if (enums[k].enumName.compare(foundVariab.name) == 0)
							{
								if ((enums[k].enumElements.size() <= 2) && (constraint1.elem2.compare(constraint2.elem2) != 0))
								{
									return true;
								}
								else
								{
									return false;
								}
							}
						}
					}
				}
				else
				{
					char constraint1ValChar;
					char constraint2ValChar;
					int constraint1ValInt;
					int constraint2ValInt;
					float constraint1ValFloat;
					float constraint2ValFloat;
					//when the first constraint is equal and all of the options for the second
					if ((constraint1.op.compare(EQ) == 0) && (constraint2.op.compare(EQ) == 0))
					{
						//std::cout << " \n EQ and EQ";
						//when check if both equal, both constraints have to be compared to null
						if (constraint1CompToNull)
						{
							if (constraint2CompToNull)
							{
								return false;
							}
							else
							{
								return true;
							}
						}
						else
						{
							if (constraint1.elem2.compare(constraint2.elem2) == 0)
							{
								return false;
							}
							else
								return true;
						}
					}
					else if ((constraint1.op.compare(EQ) == 0) && (constraint2.op.compare(DIFF) == 0))
					{
						//std::cout << " \n EQ and DIFF";
						if (constraint1CompToNull)
						{
							if (constraint2CompToNull)
							{
								return false;
							}
							else
							{
								return true;
							}
						}
						else
						{
							if (constraint1.elem2.compare(constraint2.elem2) == 0)
							{
								return true;
							}
							else
								return false;
						}
					}
					else if ((constraint1.op.compare(EQ) == 0) && (constraint2.op.compare(GREQ) == 0))
					{
						if (constraint1CompToNull)
						{
							return true;
						}
						else {
							if (varType.compare("char") == 0)
							{
								constraint1ValChar = constraint1.elem2[1];
								constraint2ValChar = constraint2.elem2[1];
								if (constraint1ValChar >= constraint2ValChar)
								{
									return true;
								}
							}
							else if (varType.compare("int") == 0)
							{
								constraint1ValInt = stoi(constraint1.elem2);
								constraint2ValInt = stoi(constraint2.elem2);
								if (constraint1ValInt >= constraint2ValInt)
								{
									return true;
								}
							}
							else if (varType.compare("float") == 0)
							{
								constraint1ValFloat = stof(constraint1.elem2);
								constraint2ValFloat = stof(constraint2.elem2);
								if (constraint1ValFloat >= constraint2ValFloat)
								{
									return true;
								}
							}
						}

					}
					else if ((constraint1.op.compare(EQ) == 0) && (constraint2.op.compare(LSEQ) == 0))
					{
						if (constraint1CompToNull)
						{
							return true;
						}
						else
						{
							if (varType.compare("char") == 0)
							{
								constraint1ValChar = constraint1.elem2[1];
								constraint2ValChar = constraint2.elem2[1];
								if (constraint1ValChar <= constraint2ValChar)
								{
									return true;
								}
							}
							else if (varType.compare("int") == 0)
							{
								constraint1ValInt = stoi(constraint1.elem2);
								constraint2ValInt = stoi(constraint2.elem2);
								if (constraint1ValInt <= constraint2ValInt)
								{
									return true;
								}
							}
							else if (varType.compare("float") == 0)
							{
								constraint1ValFloat = stof(constraint1.elem2);
								constraint2ValFloat = stof(constraint2.elem2);
								if (constraint1ValFloat <= constraint2ValFloat)
								{
									return true;
								}
							}
						}
					}
					else if ((constraint1.op.compare(EQ) == 0) && (constraint2.op.compare(GR) == 0))
					{
						if (constraint1CompToNull)
						{
							return true;
						}
						else
						{
							if (varType.compare("char") == 0)
							{
								constraint1ValChar = constraint1.elem2[1];
								constraint2ValChar = constraint2.elem2[1];
								if (constraint1ValChar > constraint2ValChar)
								{
									return true;
								}
							}
							else if (varType.compare("int") == 0)
							{
								constraint1ValInt = stoi(constraint1.elem2);
								constraint2ValInt = stoi(constraint2.elem2);
								if (constraint1ValInt > constraint2ValInt)
								{
									return true;
								}
							}
							else if (varType.compare("float") == 0)
							{
								constraint1ValFloat = stof(constraint1.elem2);
								constraint2ValFloat = stof(constraint2.elem2);
								if (constraint1ValFloat > constraint2ValFloat)
								{
									return true;
								}
							}
						}
					}
					else if ((constraint1.op.compare(EQ) == 0) && (constraint2.op.compare(LS) == 0))
					{
						if (constraint1CompToNull)
						{
							return true;
						}
						else
						{
							if (varType.compare("char") == 0)
							{
								constraint1ValChar = constraint1.elem2[1];
								constraint2ValChar = constraint2.elem2[1];
								if (constraint1ValChar < constraint2ValChar)
								{
									return true;
								}
							}
							else if (varType.compare("int") == 0)
							{
								constraint1ValInt = stoi(constraint1.elem2);
								constraint2ValInt = stoi(constraint2.elem2);
								if (constraint1ValInt < constraint2ValInt)
								{
									return true;
								}
							}
							else if (varType.compare("float") == 0)
							{
								constraint1ValFloat = stof(constraint1.elem2);
								constraint2ValFloat = stof(constraint2.elem2);
								if (constraint1ValFloat < constraint2ValFloat)
								{
									return true;
								}
							}
						}
					}


					//when the first constraint is diff and all of the options for the second


					//when the first constraint is greater or equal and all of the options for the second
					else if ((constraint1.op.compare(GREQ) == 0) && (constraint2.op.compare(LSEQ) == 0))
					{
						if (constraint1CompToNull)
						{
							return true;
						}
						else
						{
							if (varType.compare("char") == 0)
							{
								constraint1ValChar = constraint1.elem2[1];
								constraint2ValChar = constraint2.elem2[1];
								if (constraint1ValChar <= constraint2ValChar)
								{
									return true;
								}
							}
							else if (varType.compare("int") == 0)
							{
								constraint1ValInt = stoi(constraint1.elem2);
								constraint2ValInt = stoi(constraint2.elem2);
								if (constraint1ValInt <= constraint2ValInt)
								{
									return true;
								}
							}
							else if (varType.compare("float") == 0)
							{
								constraint1ValFloat = stof(constraint1.elem2);
								constraint2ValFloat = stof(constraint2.elem2);
								if (constraint1ValFloat <= constraint2ValFloat)
								{
									return true;
								}
							}
						}
					}
					else if ((constraint1.op.compare(GREQ) == 0) && (constraint2.op.compare(LS) == 0))
					{
						if (constraint1CompToNull)
						{
							return true;
						}
						else
						{
							if (varType.compare("char") == 0)
							{
								constraint1ValChar = constraint1.elem2[1];
								constraint2ValChar = constraint2.elem2[1];
								if (constraint1ValChar < constraint2ValChar)
								{
									return true;
								}
							}
							else if (varType.compare("int") == 0)
							{
								constraint1ValInt = stoi(constraint1.elem2);
								constraint2ValInt = stoi(constraint2.elem2);
								if (constraint1ValInt < constraint2ValInt)
								{
									return true;
								}
							}
							else if (varType.compare("float") == 0)
							{
								constraint1ValFloat = stof(constraint1.elem2);
								constraint2ValFloat = stof(constraint2.elem2);
								if (constraint1ValFloat < constraint2ValFloat)
								{
									return true;
								}
							}
						}
					}

					//when the first constraint is less than equal and all of the options for the second
					else if (((constraint1.op.compare(LSEQ) == 0) && (constraint2.op.compare(GR) == 0)) || ((constraint1.op.compare(LS) == 0) && (constraint2.op.compare(GR) == 0)))
					{
						if (constraint1CompToNull)
						{
							return true;
						}
						else
						{
							if (varType.compare("char") == 0)
							{
								constraint1ValChar = constraint1.elem2[1];
								constraint2ValChar = constraint2.elem2[1];
								if (constraint1ValChar > constraint2ValChar)
								{
									return true;
								}
							}
							else if (varType.compare("int") == 0)
							{
								constraint1ValInt = stoi(constraint1.elem2);
								constraint2ValInt = stoi(constraint2.elem2);
								if (constraint1ValInt > constraint2ValInt)
								{
									return true;
								}
							}
							else if (varType.compare("float") == 0)
							{
								constraint1ValFloat = stof(constraint1.elem2);
								constraint2ValFloat = stof(constraint2.elem2);
								if (constraint1ValFloat > constraint2ValFloat)
								{
									return true;
								}
							}
						}
					}
				}
				//if(variabType.compare(""))
			}
			else if (!checkIfProgramVariable(constraint1.elem1) && checkIfProgramVariable(constraint1.elem2))
			{
				//std::cout << "\n Second";

			}
		}
		else {
			//return false;
		}
	}
	return false;
}

bool ConstraintSolver::checkIfProgramVariable(std::string constraintElement)
{
	for (int i = 0; i < programVariables.size(); i++)
	{
		if (programVariables[i].name.compare(constraintElement) == 0)
		{
			//std::cout << "  " << constraintElement << " is prog var \n";
			return true;
		}
	}
	return false;
}
