#include "narray.h"
#include "nmatrix.h"
#include "neuroproject.h"
#include "inlayer.h"

// Перегрузка операций NMatrix
template <typename NType>
NMatrix<NType>& operator+(const NMatrix<NType>& A, const NMatrix<NType>& B)
{
    NMatrix<NType> C = new NMatrix<NType>(A); // C = A
    return C.sum(B); // C = C + B
}

template <typename NType>
NMatrix<NType>& operator-(const NMatrix<NType>& A, const NMatrix<NType>& B)
{
    NMatrix<NType> C = new NMatrix<NType>(A);
    return C.sum(-1 * B);
}

template <typename NType>
NMatrix<NType>& operator*(const NMatrix<NType>& A, const NMatrix<NType>& B)
{
    NMatrix<NType> C = new NMatrix<NType>();
    return C.mul(A, B);
}

template <typename NType>
NMatrix<NType>& operator+(const NMatrix<NType>& A, const NType& B)
{
    NMatrix<NType> C = new NMatrix<NType>(A);
    return C.valsum(B);
}

template <typename NType>
NMatrix<NType>& operator*(const NMatrix<NType>& A, const NType& B)
{
    NMatrix<NType> C = new NMatrix<NType>(A);
    return C.valmul(B);
}

template <typename NType>
NMatrix<NType>& operator+(const NType& B, const NMatrix<NType>& A)
{
    NMatrix<NType> C = new NMatrix<NType>(A);
    return C.valsum(B);
}

template <typename NType>
NMatrix<NType>& operator*(const NType& B, const NMatrix<NType>& A)
{
    NMatrix<NType> C = new NMatrix<NType>(A);
    return C.valmul(B);
}

// Перегрузка операций NArray
template <typename NType>
NArray<NType>& operator+(const NArray<NType>& A, const NType& B)
{
    NArray<NType> C = new NArray<NType>(A); // C = A
    return C.sum(B); // C = C + B
}

template <typename NType>
NArray<NType>& operator+(const NType& B, const NArray<NType>& A)
{
    NArray<NType> C = new NArray<NType>(A); // C = A
    return C.sum(B); // C = C + B
}

template <typename NType>
NArray<NType>& operator*(const NArray<NType>& A, const NType& B)
{
    NArray<NType> C = new NArray<NType>(A); // C = A
    return C.mul(B); // C = C * B
}

template <typename NType>
NArray<NType>& operator*(const NType& B, const NArray<NType>& A)
{
    NArray<NType> C = new NArray<NType>(A); // C = A
    return C.mul(B); // C = C * B
}

template <typename NType>
NArray<NType>& operator*(const NArray<NType>& A, const NMatrix<NType>& B)
{
    NArray<NType> C = new NArray<NType>();
    return C.mul(A, B); // C = A * B
}

template <typename NType>
NArray<NType>& operator*(const NMatrix<NType>& B, const NArray<NType>& A)
{
    NArray<NType> C = new NArray<NType>();
    return C.mul(A, B); // C = A * B
}
