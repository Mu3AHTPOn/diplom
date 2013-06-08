//---------------------------------------------------------------------------

#pragma hdrstop

#include "Project.h"

#pragma once
//---------------------------------------------------------------------------

Project::Project(): name(L"Новый проект")
{

}

Project::~Project()
{

}

int Project::getCriteriaCount()
{
	return criteriaNames.size();
}

int Project::getAlternativesCount()
{
	return alternativeEstimates.size();
}

const UnicodeString & Project::getName()
{
    return name;
}

void Project::setName(UnicodeString name)
{
    this->name = name;
}

vector<UnicodeString> Project::getAlternativeNames()
{
	vector<UnicodeString> names(getAlternativesCount());
	for (int i = 0; i < getAlternativesCount(); ++i)
	{
		names.push_back(alternativeEstimates[i].getName());
	}

	return names;
}

vector<UnicodeString> & Project::getCriteriaNames()
{
	return criteriaNames;
}

vector<Estimates> & Project::getAlternativeEstimates()
{
	return alternativeEstimates;
}

Estimates& Project::getCriteriaEstimates()
{
	return criteriaEstimates;
}

