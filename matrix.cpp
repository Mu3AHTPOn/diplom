//---------------------------------------------------------------------------

#pragma hdrstop

#include "matrix.h"
//---------------------------------------------------------------------------
template<typename T>
Matrix<T>::Matrix(int height, int width, T defaultValue)
{
    this->height = height;
    this->width = width;

    initMatrix(defaultValue);
}

template<typename T>
Matrix<T>::Matrix(Matrix<T> &rhs)
{
    height = rhs.getHeight();
    width = rhs.getWidth();

    initMatrix(0);

    for (int i(0); i < height; ++i)
    {
        for(int j(0); j < width; ++j)
        {
            matrix[i][j] = rhs[i][j];
        }
    }
}

template<typename T>
Matrix<T>::~Matrix()
{
    for (int i = 0; i < width; ++i)
    {
        delete [] matrix[i];
    }
    delete [] matrix;
}

template<typename T>
int Matrix<T>::getWidth() const
{
    return width;
}

template<typename T>
int Matrix<T>::getHeight() const
{
    return height;
}

template<typename T>
T *Matrix<T>::eigenVectors()
{

}

template<typename T>
T* Matrix<T>::operator [](const int row)
{
    return matrix[row];
}

template<typename T>
Matrix<T>& Matrix<T>::getColumn(const int index)
{
    Matrix<T>& column = *(new Matrix<T>(getHeight()));
    for (int i = 0; i < getHeight(); ++i)
    {
        column[i][0] = matrix[i][index];
    }

    return column;
}

template<typename T>
Matrix<T>& Matrix<T>::operator *(Matrix<T>& rhs) throw(MatrixException)
{
    if (getWidth() != rhs.getHeight())
    {

    }

    Matrix<T> *result = new Matrix<T>(getHeight(), rhs.getWidth());
    for (int k(0); k < result->getHeight(); ++k)
    {
        for (int l(0); l < result->getWidth(); ++l)
        {
            for (int j(0); j < rhs.getHeight(); ++j)
            {
                (*result)[k][l] += matrix[k][j] * rhs[j][l];
            }
        }
    }

    return *result;
}

template<typename T>
Matrix<T>& Matrix<T>::operator =(Matrix<T> &rhs)
{
    if (this == &rhs)
    {
        return *this;
    }

    width = rhs.getWidth();
    height = rhs.getHeight();
    initMatrix(0);
    for (int i = 0; i < getHeight(); ++i)
    {
        for (int j = 0; j < getWidth(); ++j)
        {
            (*this)[i][j] = rhs[i][j];
        }
    }

    return *this;
}

template<typename T>
Matrix<T> &Matrix<T>::operator /(double value)
{
    Matrix<T> &result = *(new Matrix<T>(*this));

    for (int i(0); i < height; ++i)
    {
        for (int j(0); j < width; ++j)
        {
            result[i][j] = matrix[i][j] / value;
        }
    }

    return result;
}

//добавление к матрице другой матрицы
template<typename T>
Matrix<T> &Matrix<T>::append(Matrix<T> &rhs) throw(MatrixException)
{
    int newWidth;
    int newHeight;

    if (getHeight() == rhs.getHeight())
    {
        newWidth = getWidth() + rhs.getWidth();
        newHeight = getHeight();
    }
    else if (getWidth() == rhs.getWidth())
    {
        newHeight = getHeight() + rhs.getHeight();
        newWidth = getWidth();
    }
    else
    {
        throw MatrixException(MatrixException::ERROR_WRONG_SIZE);
    }

    T **newMatrix = new T*[newHeight];
    for (int i = 0; i < newHeight; ++i)
    {
        newMatrix[i] = new T[newWidth];
    }

    for (int i = 0; i < getHeight(); ++i)
    {
        for (int j = 0; j < getWidth(); ++j)
        {
            newMatrix[i][j] = matrix[i][j];
        }
    }

    int initI, initJ;
    if (rhs.getHeight() == getHeight())
    {
        initI = 0;
        initJ = getWidth();
    }
    else
    {
        initI = getHeight();
        initJ = 0;
    }

    int k(0), l(0);
    for (int i(initI); i < newHeight; ++i)
    {
        l = 0;
        for (int j(initJ); j < newWidth; ++j)
        {
            newMatrix[i][j] = rhs[k][l];
            ++l;
        }
        ++k;
    }

    width = newWidth;
    height = newHeight;
    matrix = newMatrix;
    return *this;
}


template<typename T>
bool Matrix<T>::isQuadratic()
{
    return width == height;
}

template<typename T>
void Matrix<T>::initMatrix(T value)
{
    matrix = new T*[getHeight()];
    for (int i = 0; i < getHeight(); ++i)
    {
        matrix[i] = new T[getWidth()];
        for (int j = 0; j < getWidth(); ++j)
        {
            matrix[i][j] = value;
        }
    }
}

