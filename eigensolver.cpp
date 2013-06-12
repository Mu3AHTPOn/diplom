#include "matrix.h"
#include "eigensolver.h"

//нахождение собственных показателей матрицы
template<typename T>
void EigenSolver::evaluateEigen(
        Matrix<T> &inMatrix,
        Matrix<double>& outMaxVector,
        double& outMaxEigenValue)
{
    evaluateEigenVector(inMatrix, outMaxVector);
    evaluateMaxEigenValue(inMatrix, outMaxVector, outMaxEigenValue);
}
//вычисляет собственные вектора
template<typename T>
void EigenSolver::evaluateEigenVector(
        Matrix<T>& inMatrix,
        Matrix<double>& outMaxVector)
{
    /* нахождение собственного вектора, соответствующего макисмальному собственному числу
    * 1. суммируем элементы каждой строки и записываем полученные результаты в
    *   столбец
    * 2. складываем все элементы найденного столбца
    * 3. делим каждый из элементов на этого столбца на полученную сумму
    */
        Matrix<double> columnsSum(inMatrix.getHeight());
        for (int i = 0; i < inMatrix.getHeight(); ++i)
        {
            for (int j = 0; j < inMatrix.getWidth(); ++j)
            {
                columnsSum[i][0] += inMatrix[i][j];
            }
        }

        T sum(0);
        for (int i = 0; i < inMatrix.getHeight(); ++i)
        {
            sum += columnsSum[i][0];
        }

        for (int i = 0; i < inMatrix.getHeight(); ++i)
        {
            columnsSum[i][0] /= (double) sum;
        }

        outMaxVector = columnsSum;
}

//нахождение максимального собственного числа
template<typename T>
void EigenSolver::evaluateMaxEigenValue(
        Matrix<T>& inMatrix,
        Matrix<double>& outMaxVector,
        double& outMaxEigenValue)
{
	Matrix<double> *y = inMatrix * outMaxVector;
//    Matrix<double>& refY = *y;

	for (int i = 0; i < y->getHeight(); ++i)
	{
		outMaxEigenValue += (*y)[i][0] / (double) outMaxVector[i][0];
	}

	outMaxEigenValue /= y->getHeight();

	delete y;
}

