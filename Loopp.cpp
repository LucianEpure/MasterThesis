#include "Loopp.h"
Loopp::Loopp()
{
	this->constantCount = true;
}

void Loopp::findLoopVariables(std::vector<Variab> programVariabs)
{
	std::vector<Variab> loopVariables;
	std::vector<std::string> line;
	for (int i = 0; i < this->loopContent.size(); i++)
	{
		std::istringstream ss(loopContent[i]);
		std::string word; // for storing each word
		while (ss >> word)
		{
			line.push_back(word);
		}
		for (int j = 0; j < programVariabs.size(); j++)
		{
			for (int k = 0; k < line.size(); k++)
			{
				if ((programVariabs[j].name.compare(line[k]) == 0) && !checkIfAlreadyInVarList(line[k]))
				{
					Variab loopVariable;
					loopVariable.isEnum = programVariabs[j].isEnum;
					loopVariable.name = programVariabs[j].name;
					loopVariable.location = programVariabs[j].location;
					loopVariable.type = programVariabs[j].type;
					this->loopVars.push_back(loopVariable);
				}
			}
		}
		line.clear();
	}
}

bool Loopp::checkIfAlreadyInVarList(std::string variable)
{
	if (!this->loopVars.empty())
	{
		for (int i = 0; i < this->loopVars.size(); i++)
		{
			if (this->loopVars[i].name.compare(variable) == 0)
			{
				return true;
			}
		}
	}
	return false;
}

void Loopp::getPossibleStateVars()
{

	std::vector<std::string> line;
	for (int i = 0; i < this->loopVars.size(); i++)
	{
		if (this->loopVars[i].isEnum || this->loopVars[i].type.compare("int") || this->loopVars[i].type.compare("int*") || this->loopVars[i].type.compare("char") || this->loopVars[i].type.compare("char*"))
		{
			Variab possibleStateVar = this->loopVars[i];
			this->possibleStateVars.push_back(possibleStateVar);
		}
	}
}