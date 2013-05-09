#ifndef MATRIXEXCEPTION_H
#define MATRIXEXCEPTION_H

#include<exception>

class MatrixException : public std::exception
{
public:
    enum ErrorCodes
    {
        ERROR_WRONG_SIZE,
        ERRRO_WRONG_INDEX
    };

    MatrixException(ErrorCodes code): std::exception(), _code(code) {}
    const char* what() const throw();
private:
    ErrorCodes _code;
};

#endif // MATRIXEXCEPTION_H

