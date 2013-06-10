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
	return alternativeNames.size();
}

const UnicodeString & Project::getName()
{
    return name;
}

void Project::setName(UnicodeString name)
{
    this->name = name;
}

vector<UnicodeString> &Project::getAlternativeNames()
{
//	vector<UnicodeString> names(getAlternativesCount());
//	const int n(getAlternativesCount());
//	for (int i = 0; i < n; ++i)
//	{
//		UnicodeString str = alternativeEstimates[i].getEstimatedBy();
//		names.push_back(str);
//	}

	return alternativeNames;
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
    	alternativeNames.erase(alternativeNames.begin() + index);

		vector<Estimates>::iterator estimatesIter = alternativeEstimates.begin();
		while (estimatesIter != alternativeEstimates.end()) {
			vector< vector<int> > & rates = estimatesIter->getRates();
			vector< vector<int> >::iterator iter = rates.begin();
			while (iter != rates.end()) {
				iter->erase(iter->begin() + index);
				++iter;
			}

			rates.erase(rates.begin() + index);
			vector<double> &priorities = estimatesIter->getPriorities();
			priorities.erase(priorities.begin() + index);
			++estimatesIter;
		}
	} else {
		criteriaNames.erase(criteriaNames.begin() + index);

		vector< vector<int> > & rates = criteriaEstimates.getRates();
		vector<double> &priorities = criteriaEstimates.getPriorities();
		vector< vector<int> >::iterator iter = rates.begin();
		while (iter != rates.end()) {
			iter->erase(iter->begin() + index);
			++iter;
		}

		rates.erase(rates.begin() + index);
		priorities.erase(priorities.begin() + index);
		alternativeEstimates.erase(alternativeEstimates.begin() + index);
	}
}

void Project::addEstimate(int index, UnicodeString name, EstimateType type)
{
	 if (type == EstimateType::ALTERNATIVE) {
		 if (index < 0) {
			alternativeNames.push_back(name);
		} else {
			alternativeNames.insert(alternativeNames.begin() + index, name);
		}

		//добавление столбцов новых оценок
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
				estimatesIter->getPriorities().push_back(0.0);
			} else {
				rates.insert(rates.begin() + index, vector<int>(rates.size() + 1, 0));
				estimatesIter->getPriorities().insert(estimatesIter->getPriorities().begin() + index, 0.0);
			}

			++estimatesIter;
		}
	} else {

		if (index < 0) {
			criteriaNames.push_back(name);
		} else {
			criteriaNames.insert(criteriaNames.begin() + index, name);
        }

		//добавление столбца баллов для нового критерия
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

		//добавлние строки баллов для нового критерия
		if (index < 0) {
			rates.push_back(vector<int>(rates.size() + 1, 0));
            criteriaEstimates.getPriorities().push_back(0.0);
		} else {
			rates.insert(rates.begin() + index, vector<int>(rates.size() + 1, 0));
			criteriaEstimates.getPriorities().insert(criteriaEstimates.getPriorities().begin(), 0.0);
		}

		//добавление оценок альтернатив по новому критерию
		Estimates newEstimates;
//		if (alternativeEstimates.size() > 0) {
			unsigned int n = getAlternativesCount();
			newEstimates.getRates().resize(n, vector<int>(n, 0));
			newEstimates.getPriorities().resize(n, 0.0);
//		} else {
//			newEstimates.getRates().resize(1, vector<int>(1, 0));
//			newEstimates.getPriorities().resize(1, 0.0);
//		}
		if (index < 0) {
			alternativeEstimates.push_back(newEstimates);
		} else {
			alternativeEstimates.insert(alternativeEstimates.begin() + index, newEstimates);
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
