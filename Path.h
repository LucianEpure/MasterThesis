#pragma once
#include<string>
#include<vector>
#include"Assig.h"
#include"Constraint.h"
#include"Constants.h"
class Path
{
public:
	Path();

	Assig assig1;
	Assig assig2;
	std::vector<Constraint> pathConstraints;
};