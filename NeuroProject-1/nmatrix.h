#ifndef NMATRIX_H
#define NMATRIX_H

#include "narray.h"

template <typename NType> class NArray;

template <typename NType>
class NMatrix
{
public:
    NMatrix();
    NMatrix(NMatrix<NType> &obj);
    NMatrix<NType>& operator=(NMatrix<NType>& obj);
    ~NMatrix();
protected:
    NType* data;
    int lenRow;
    int lenColumn;
    int sizeRow;
    int sizeColumn;
public:
    void init(int lenRow, int lenColumn);
    void init(int lenRow, int lenColumn, NType value);
    void clear();
    void addRow(int pos, const NType &value);
    void delRow(int pos);
    void addColumn(int pos, const NType &value);
    void delColumn(int pos);
    void set(const NType &element, int posRow, int posColumn);
    NType get(int posRow, int posColumn);
    NType* getData();
    void resize(int sizeRow, int sizeColumn);
    void renew(int sizeRow, int sizeColumn);
    int getLenRow();
    int getLenColumn();
    int getSizeRow();
    int getSizeColumn();
public:
    NMatrix<NType>& sum(NMatrix<NType>& B);
    NMatrix<NType>& sum(NMatrix<NType>& A, NMatrix<NType>& B);
    NMatrix<NType>& valsum(NType& B);
    NMatrix<NType>& mul(NArray<NType>& A, NArray<NType>& B);
    NMatrix<NType>& mul(NMatrix<NType>& B);
    NMatrix<NType>& mul(NMatrix<NType>& A, NMatrix<NType>& B);
    NMatrix<NType>& matmul(NMatrix<NType>& B);
    NMatrix<NType>& valmul(NType& B);
};

#endif // NMATRIX_H
