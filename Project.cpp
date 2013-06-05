//---------------------------------------------------------------------------

#pragma hdrstop

#include "Project.h"
//---------------------------------------------------------------------------

Project::Project(): name(L"����� ������")
{

}

Project::~Project()
{
	criteriaNames.clear();
	alternativeNames.clear();
}

int Project::getCriteriaCount()
{
    return criteriaNames.size();
}

int Project::getAlternativesCount()
{
    return alternativeNames.size();
}

void Project::setName(UnicodeString & name)
{
    this->name = name;
}

vector<UnicodeString> & Project::getAlternativeNames()
{
    return alternativeNames;
}

vector<UnicodeString> & Project::getCriteriaNames()
{
	return criteriaNames;
}