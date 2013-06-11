//---------------------------------------------------------------------------

#ifndef matrixH
#define matrixH
//---------------------------------------------------------------------------

#include "matrixexception.h"

template <typename T>
class Matrix
{
	public:
		Matrix() {}
        Matrix(int height, int width = 1, T defaultValue = 0);
        Matrix(Matrix<T> &rhs);
        ~Matrix();

        inline bool isQuadratic();
        inline void setWidth(const int width);
        inline void setHeight(const int height);
        T* operator[](const int row);
        Matrix<T>& getColumn(const int index);
        Matrix<T>* operator *(Matrix<T>& rhs) throw(MatrixException);
		Matrix<T>& operator =(Matrix<T>& rhs);
        Matrix<T>& operator /(double value);
		Matrix<T>& append(Matrix<T>& rhs) throw(MatrixException);
		Matrix<T>& normalizeToOne();
		double getLength(int col);
		T getSum(int col);

        inline int getWidth() const;
        inline int getHeight() const;
        T** getMatrix() const { return matrix; }
        T * eigenVectors();
        T * eigenValue();
        T * eigenValue(T *eigenValue);
	private:
        T **matrix;
        void initMatrix(T value);
        int width;
        int height;

};

#endif

