#include "Variab.h"
Variab::Variab()
{
	this->isEnum = false;
	this->isInt = false;
}

std::string Variab::getProgramVariable()
{
	return this->name + " " + this->type + " " + this->location;
}