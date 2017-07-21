#ifndef NARRAY_H
#define NARRAY_H

#include "nmatrix.h"

template <typename NType> class NMatrix;

template <typename NType>
class NArray
{
public:
    NArray();
    NArray(NArray<NType> &obj);
    NArray<NType>& operator=(NArray<NType>& obj);
    NArray(int size);
    NArray(int size, int length, int block);
    ~NArray();
protected:
    int length;
    int size;
    int block;
    NType* data;
public:
    void init(int length);
    void init(int length, NType value);
    void clear();
    void add(const NType& element, int pos);
    NType del(int pos);
    void push(const NType& element);
    NType pop();
    void set(const NType& element, int pos);
    NType get(int pos);
    void setBlock(int block);
    int getBlock();
    int getLength();
    int getSize();
    void resize(int size);
    void renew(int size);
    NType* getData();
public:
    NArray<NType>& valsum(NType& B);
    NArray<NType>& sum(NArray<NType>& B);
    NArray<NType>& sum(NArray<NType>& A, NArray<NType>& B);
    NArray<NType>& valmul(NType& B);
    NArray<NType>& mul(NArray<NType>& B);
    NArray<NType>& mul(NArray<NType>& A, NArray<NType>& B);
    NArray<NType>& mul(NMatrix<NType>& B, bool orient);
    NArray<NType>& mul(NArray<NType>& A, NMatrix<NType>& B, bool orient);
    NArray<NType>& mul(NMatrix<NType>& B, NArray<NType>& A, bool orient);
};

#endif // NARRAY_H
