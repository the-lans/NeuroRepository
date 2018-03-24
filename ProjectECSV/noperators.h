#ifndef NOPERATORS_H
#define NOPERATORS_H

#include "narray.h"
#include "nmatrix.h"


// Перегрузка операций NMatrix
template <typename NType>
bool operator==(const NMatrix<NType>& left, const NMatrix<NType>& right)
{
    if(left.getLenRow() != right.getLenRow() || left.getLenColumn() != right.getLenColumn()) {return false;}
    NType* pL = left.getData();
    NType* pR = right.getData();
    int sizeColumn = left.getSizeColumn();
    int index;

    int i, j;
    for(i = 0; i < left.getLenRow(); i++)
    {
        for(j = 0; j < left.getLenColumn(); j++)
        {
            index = i * sizeColumn + j;
            if(pL[index] != pR[index]) {return false;}
        }
    }
    return true;
}

template <typename NType>
NMatrix<NType> operator+(const NMatrix<NType>& A, const NMatrix<NType>& B)
{
    NMatrix<NType> C(A); // C = A
    return C.sum(B); // C = C + B
}

template <typename NType>
NMatrix<NType> operator-(const NMatrix<NType>& A, const NMatrix<NType>& B)
{
    NMatrix<NType> C(A);
    return C.sum(B.valsign());
}

template <typename NType>
NMatrix<NType> operator*(const NMatrix<NType>& A, const NMatrix<NType>& B)
{
    NMatrix<NType> C(A);
    return C.mul(B, true);
}

template <typename NType>
NMatrix<NType> operator+(const NMatrix<NType>& A, const NType& B)
{
    NMatrix<NType> C(A);
    return C.valsum(B);
}

template <typename NType>
NMatrix<NType> operator*(const NMatrix<NType>& A, const NType& B)
{
    NMatrix<NType> C(A);
    return C.valmul(B);
}

template <typename NType>
NMatrix<NType> operator+(const NType& B, const NMatrix<NType>& A)
{
    NMatrix<NType> C(A);
    return C.valsum(B);
}

template <typename NType>
NMatrix<NType> operator*(const NType& B, const NMatrix<NType>& A)
{
    NMatrix<NType> C(A);
    return C.valmul(B);
}

// Перегрузка операций NArray
template <typename NType>
bool operator==(const NArray<NType>& left, const NArray<NType>& right)
{
    if(left.getLength() != right.getLength()) {return false;}
    NType* pL = left.getData();
    NType* pR = right.getData();

    for(int i = 0; i < left.getLength(); i++)
    {
        if(pL[i] != pR[i]) {return false;}
    }
    return true;
}

template <typename NType>
NArray<NType> operator+(const NArray<NType>& A, const NType& B)
{
    NArray<NType> C(A); // C = A
    return C.valsum(B); // C = C + B
}

template <typename NType>
NArray<NType> operator+(const NType& B, const NArray<NType>& A)
{
    NArray<NType> C(A); // C = A
    return C.valsum(B); // C = C + B
}

template <typename NType>
NArray<NType> operator*(const NArray<NType>& A, const NType& B)
{
    NArray<NType> C(A); // C = A
    return C.valmul(B); // C = C * B
}

template <typename NType>
NArray<NType> operator*(const NType& B, const NArray<NType>& A)
{
    NArray<NType> C(A); // C = A
    return C.valmul(B); // C = C * B
}

template <typename NType>
NArray<NType> operator/(const NArray<NType>& A, const NType& B)
{
    NArray<NType> C(A); // C = A
    return C.valmul(B); // C = C * B
}

template <typename NType>
NArray<NType> operator*(const NArray<NType>& A, const NMatrix<NType>& B)
{
    NArray<NType> C(A); // C = A
    return C.mul(B, true); // C = C * B
}

template <typename NType>
NArray<NType> operator*(const NMatrix<NType>& B, const NArray<NType>& A)
{
    NArray<NType> C(A); // C = A
    return C.mul(B, true); // C = C * B
}

#endif // NOPERATORS_H
