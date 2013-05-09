#ifndef EIGENSOLVER_H
#define EIGENSOLVER_H

#include "matrix.h"

class EigenSolver
{
public:
    EigenSolver() {};

    template<typename T>
    void static evaluateEigen(Matrix<T>& inMatrix,
                       Matrix<double>& outMaxVector,
                       double& outMaxEigenValue);
private:
    template<typename T>
    void static evaluateEigenVector(Matrix<T>& inMatrix, Matrix<double>& outMaxVector);
    template<typename T>
    void static evaluateMaxEigenValue(Matrix<T>& inMatrix,
                               Matrix<double>& outMaxVector,
                               double& outMaxEigenValue);

};

#endif

