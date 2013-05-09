#include "ahpsolver.h"

// ��������� ���
template<typename T>
AHPSolver<T>::AHPSolver(Matrix<T> &esimates)
{
    //�������������� ����������� ��������������
    maxEigenValue = new double(0);
    maxEigenVector = new Matrix<double>(1);
    //���������� ������� ������ ���������
    computePairwiseComparationMatrix(esimates);
    //���������� ����������� ������������� ������� ������ ���������
    EigenSolver::evaluateEigen(
                (*pairwiseComparation),
                (*maxEigenVector),
                (*maxEigenValue)
            );

    //��������� �������� ���������

}

template<typename T>
AHPSolver<T>::~AHPSolver()
{
    delete pairwiseComparation;
    delete maxEigenVector;
    delete maxEigenValue;
    delete weights;
}

template<typename T>
Matrix<double> &AHPSolver<T>::getPairwiseComparationMatrix()
{
    return *pairwiseComparation;
}

//������ ������� ���������������
template<typename T>
double AHPSolver<T>::evaluatePairwiseConsistency()
{
	const int size = pairwiseComparation->getHeight();
    return ((*maxEigenValue) - size) / (double) (size - 1);
}

//double AHPSolver::evaluatePairwiseConsistency()
//{
//    return evaluateConsistency(*pairwiseComparation, *maxEigenValue);
//}

template<typename T>
Matrix<double> &AHPSolver<T>::getMaxEigenVectors()
{
    return (*maxEigenVector);
}

template<typename T>
double &AHPSolver<T>::getMaxEigenValue()
{
    return (*maxEigenValue);
}

//���������� ���������� ������� ������ ��������� (���)
template<typename T>
void AHPSolver<T>::computePairwiseComparationMatrix(Matrix<T> &criteriaEstimates)
{
    const int size = criteriaEstimates.getHeight();
    pairwiseComparation = new Matrix<double>(size, size);
    for (int i = 0; i < size; ++i)
    {
        for(int j = 0; j < size; ++j)
        {
            (*pairwiseComparation)[i][j] =
                criteriaEstimates[i][0] / (double) criteriaEstimates[j][0];
        }
    }
}

