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
	const int n(getAlternativesCount());
	for (int i = 0; i < n; ++i)
	{
		UnicodeString str = alternativeEstimates[i].getName();
		names.push_back(str);
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

void Project::removeEstimate(int index, EstimateType type)
{
	if (type == EstimateType::ALTERNATIVE) {
		vector<Estimates>::iterator estimatesIter = alternativeEstimates.begin();

		while (estimatesIter != alternativeEstimates.end()) {
			vector< vector<int> > & rates = estimatesIter->getRates();
			vector< vector<int> >::iterator iter = rates.begin();
			while (iter != rates.end()) {
				iter->erase(iter->begin() + index);
				++iter;
			}

			rates.erase(rates.begin() + index);
			++estimatesIter;
		}

		alternativeEstimates.erase(alternativeEstimates.begin() + index);
	} else {
		criteriaNames.erase(criteriaNames.begin() + index);

		vector< vector<int> > & rates = criteriaEstimates.getRates();
		vector< vector<int> >::iterator iter = rates.begin();
		while (iter != rates.end()) {
			iter->erase(iter->begin() + index);
			++iter;
		}

		rates.erase(rates.begin() + index);
	}
//    vector.erase(vector.begin() + i);
}

void Project::addEstimate(int index, UnicodeString name, EstimateType type)
{
     if (type == EstimateType::ALTERNATIVE) {
		vector<Estimates>::iterator estimatesIter = alternativeEstimates.begin();

		while (estimatesIter != alternativeEstimates.end()) {
			vector< vector<int> > & rates = estimatesIter->getRates();
			vector< vector<int> >::iterator iter = rates.begin();
			while (iter != rates.end()) {
				if (index < 0) {
					iter->push_back(0);
				} else {
					iter->insert(iter->begin() + index, 0);
				}

				++iter;
			}

			if (index < 0) {
				rates.push_back(vector<int>(rates.size() + 1, 0));
			} else {
				rates.insert(rates.begin() + index, vector<int>(rates.size() + 1, 0));
			}

			++estimatesIter;
		}

		Estimates newEstimates(name);
		if (alternativeEstimates.size() > 0) {
			unsigned int n = alternativeEstimates[0].getRates().size();
			newEstimates.getRates().resize(n, vector<int>(n, 0));
		} else {
			newEstimates.getRates().resize(1, vector<int>(1, 0));
        }
		if (index < 0) {
			alternativeEstimates.push_back(newEstimates);
		} else {
			alternativeEstimates.insert(alternativeEstimates.begin() + index, newEstimates);
        }
	} else {

		if (index < 0) {
			criteriaNames.push_back(name);
		} else {
			criteriaNames.insert(criteriaNames.begin() + index, name);
        }

		vector< vector<int> > & rates = criteriaEstimates.getRates();
		vector< vector<int> >::iterator iter = rates.begin();
		while (iter != rates.end()) {
			if (index < 0) {
				iter->push_back(0);
			} else {
				iter->insert(iter->begin() + index, 0);
			}
			++iter;
		}

		if (index < 0) {
			rates.push_back(vector<int>(rates.size() + 1, 0));
		} else {
            rates.insert(rates.begin() + index, vector<int>(rates.size() + 1, 0));
        }
    }
}

void Project::moveEstimate(int from, int to, EstimateType type)
{

}

void Project::setMethod(int method) {
    this->method = method;
}

int Project::getMethod() {
    return method;
}
