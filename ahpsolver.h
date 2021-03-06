#ifndef AHPSOLVER_H
#define AHPSOLVER_H

#include "matrix.h"
#include "eigensolver.cpp"

template<typename T>
class AHPSolver
{
public:
	AHPSolver();
	AHPSolver(Matrix<T> &esimates);
    ~AHPSolver();
	inline Matrix<double> & getPairwiseComparationMatrix();
	inline void setPairwiseComparationMatrix(Matrix<double> & m);
//    double evaluateConsistency(Matrix &m, double &maxEigenValue);
    double evaluatePairwiseConsistency();
    inline Matrix<double> &getMaxEigenVectors();
    inline double &getMaxEigenValue();

private:
    void computePairwiseComparationMatrix(Matrix<T> &criteriaEstimates);
    Matrix<double> *pairwiseComparation;
    Matrix<double> *maxEigenVector;
    double *maxEigenValue;
};

#endif // AHPSOLVER_H

