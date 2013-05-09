#include "matrixexception.h"

const char *MatrixException::what() const throw()
{
    switch (_code) {
    case ERROR_WRONG_SIZE:
        return "Wrong size of matrix";
        break;
    case ERRRO_WRONG_INDEX:
        return "Index out of the bounds";
        break;
    default:
        return "Unknown exception";
        break;
    }
}

