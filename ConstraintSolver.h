#include<string>
#include<iostream>
#include<sstream>   
#include<vector>
#include<algorithm>
#include"Path.h"
#include"Constraint.h"
#include"Constants.h"
#include"Variab.h"
#include"Enum.h"
class ConstraintSolver
{
public:
	ConstraintSolver();

	bool areConflicts(Path p);

	std::vector<Variab> programVariables;
	std::vector<Enum> enums;
	std::vector<Constraint> parseAndArrangeConstraints(std::vector<Constraint> pathConstraints);

private:
	std::vector<Constraint> convertConstraints(std::vector<Constraint> pathConstraints);
	bool checkIfContradict(Constraint constraint1, Constraint constraint2);
	bool checkIfProgramVariable(std::string constraintElement);

};


