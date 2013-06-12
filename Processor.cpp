//---------------------------------------------------------------------------

#pragma hdrstop

#include "Processor.h"
//---------------------------------------------------------------------------

#pragma once

AHPSolver<int> *Processor::evalAHP(Estimates &estimates) {
	Matrix<double> *m = evalPairwiseMatrix(estimates.getRates());

	AHPSolver<int> *ahpEstimates = new AHPSolver<int>();
	ahpEstimates->setPairwiseComparationMatrix(*m);

	delete m;

	Matrix<double> &eigen = ahpEstimates->getMaxEigenVectors();
	double sum(0);
	for (int i = 0; i < eigen.getHeight(); ++i)
	{
		sum += eigen[i][0];
	}

	vector<double> &priorities = estimates.getPriorities();
	priorities.clear();
	for (int i = 0; i < estimates.getRates().size(); ++i)
	{
		priorities.push_back(eigen[i][0] / sum);
	}

	return ahpEstimates;
}
//---------------------------------------------------------------------------
Matrix<double> *Processor::evalPairwiseMatrix(vector< vector<int> > &rates) {
	const int size = rates.size();
	Matrix<double> *m = new Matrix<double>(size, size);
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			(*m)[i][j] = rates[i][j] / (double) rates[j][i];
		}
	}

    return m;
}
//---------------------------------------------------------------------------
double Processor::evalConsistency(Estimates &estimates) {
	Matrix<double> *m = evalPairwiseMatrix(estimates.getRates());

	AHPSolver<int> ahpEstimates;
	ahpEstimates.setPairwiseComparationMatrix(*m);

	delete m;

	return ahpEstimates.evaluatePairwiseConsistency();
}
//---------------------------------------------------------------------------
Matrix<double> &Processor::evalWS() {
	Project &project = ProjectManager::getInstance().getCurrentProject();
	const int criteriaCount = project.getCriteriaCount();
	const int alternativesCount = project.getAlternativesCount();

	//создаём и заполняем матрицы рейтинков
	Matrix<double> criteriaPriorities(criteriaCount);
	Matrix<double> alternativePriorities(alternativesCount, criteriaCount);

	vector<double> &priorities = project.getCriteriaEstimates().getPriorities();
	vector<Estimates> &alternativeEstimates = project.getAlternativeEstimates();

	for (int i = 0; i < criteriaCount; ++i) {
		criteriaPriorities[i][0] = priorities[i];

		vector<double> &v = alternativeEstimates[i].getPriorities();
		for (int j = 0; j < alternativesCount; ++j) {
			alternativePriorities[j][i] = v[j];
		}
	}

	Matrix<double> &result = (alternativePriorities * criteriaPriorities)->normalizeToOne();
	return result;
}