#ifndef NARRAY_CPP
#define NARRAY_CPP

#include "narray.h"

template <typename NType>
NArray<NType>::NArray()
{
    this->data = nullptr;
    this->length = 0;
    this->size = 0;
    this->block = 1;
}

template <typename NType>
NArray<NType>::NArray(NArray<NType> &obj)
{
    this->data = new NType[obj.getSize()];
    this->length = obj.getLength();
    this->size = obj.getSize();
    this->block = obj.getBlock();

    NType* p = obj.getData();
    for(int i = 0; i < this->length; i++)
    {
        this->data[i] = p[i];
    }
}

template <typename NType>
NArray<NType>& NArray<NType>::operator=(NArray<NType>& obj)
{
    int len = obj.getLength();
    this->block = obj.getBlock();

    if(len > this->size)
    {
        this->renew(len + this->block);
    }
    this->length = len;

    NType* p = obj.getData();
    for(int i = 0; i < len; i++)
    {
        this->data[i] = p[i];
    }

    return *this;
}

template <typename NType>
NArray<NType>::NArray(int size)
{
    this->data = new NType[size];
    this->length = 0;
    this->size = size;
    this->block = 1;
}

template <typename NType>
NArray<NType>::NArray(int size, int length, int block)
{
    this->data = new NType[size];
    this->length = length;
    this->size = size;
    this->block = block;
}

template <typename NType>
NArray<NType>::~NArray()
{
    if(this->data != nullptr) {delete[] this->data;}
}

template <typename NType>
void NArray<NType>::init(int length)
{
    if(length > this->size)
    {
        this->resize(length);
    }

    this->length = length;
}

template <typename NType>
void NArray<NType>::init(int length, NType value)
{
    if(length > this->size)
    {
        //this->resize(length);
        this->renew(length);
    }

    this->length = length;
    for(int i = 0; i < this->length; i++)
    {
        this->data[i] = value;
    }
}

template <typename NType>
void NArray<NType>::clear()
{
    this->length = 0;
}

template <typename NType>
void NArray<NType>::add(const NType& element, int pos)
{
    if(this->length >= this->size)
    {
        this->resize(this->size + this->block);
    }
    for(int i = this->length; i > pos; i--)
    {
        this->data[i] = this->data[i-1];
    }
    this->data[pos] = element;
    this->length++;
}

template <typename NType>
NType NArray<NType>::del(int pos)
{
    this->length--;
    NType result = this->data[pos];
    for(int i = pos; i < this->length; i++)
    {
        this->data[i] = this->data[i+1];
    }
    return result;
}

template <typename NType>
void NArray<NType>::push(const NType& element)
{
    if(this->length >= this->size)
    {
        this->resize(this->size + this->block);
    }
    this->data[this->length] = element;
    this->length++;
}

template <typename NType>
NType NArray<NType>::pop()
{
    this->length--;
    return this->data[this->length];
}

template <typename NType>
void NArray<NType>::set(const NType& element, int pos)
{
    this->data[pos] = element;
}

template <typename NType>
NType NArray<NType>::get(int pos)
{
    return this->data[pos];
}

template <typename NType>
void NArray<NType>::setBlock(int block)
{
    this->block = block;
}

template <typename NType>
int NArray<NType>::getLength()
{
    return this->length;
}

template <typename NType>
int NArray<NType>::getSize()
{
    return this->size;
}

template <typename NType>
int NArray<NType>::getBlock()
{
    return this->block;
}

template <typename NType>
void NArray<NType>::resize(int size)
{
    size = size > this->length ? size : this->length;
    if(this->size != size)
    {
        NType* p = this->data;
        this->data = new NType[size];
        this->size = size;
        for(int i = 0; i < this->length; i++)
        {
            this->data[i] = p[i];
        }
        if(p != nullptr) {delete[] p;}
    }
}

template <typename NType>
void NArray<NType>::renew(int size)
{
    delete[] this->data;
    this->data = new NType[size];
    this->size = size;
}

template <typename NType>
NType* NArray<NType>::getData()
{
    return this->data;
}

template <typename NType>
NArray<NType>& NArray<NType>::valsum(NType& B)
{
    for(int i = 0; i < this->length; i++)
    {
        this->data[i] += B;
    }
    return (*this);
}

template <typename NType>
NArray<NType>& NArray<NType>::sum(NArray<NType>& B)
{
    NType* pB = B.getData();
    for(int i = 0; i < this->length; i++)
    {
        this->data[i] += pB[i];
    }
    return (*this);
}

template <typename NType>
NArray<NType>& NArray<NType>::sum(NArray<NType>& A, NArray<NType>& B)
{
    NType* pA = A.getData();
    NType* pB = B.getData();
    for(int i = 0; i < this->length; i++)
    {
        this->data[i] = pA[i] + pB[i];
    }
    return (*this);
}

template <typename NType>
NArray<NType>& NArray<NType>::valmul(NType& B)
{
    for(int i = 0; i < this->length; i++)
    {
        this->data[i] *= B;
    }
    return (*this);
}

template <typename NType>
NArray<NType>& NArray<NType>::mul(NMatrix<NType>& B, bool orient)
{
    return this->mul(*this, B, orient);
}

template <typename NType>
NArray<NType>& NArray<NType>::mul(NArray<NType>& B)
{
    return this->mul(*this, B);
}

template <typename NType>
NArray<NType>& NArray<NType>::mul(NArray<NType>& A, NArray<NType>& B)
{
    NType* pA = A.getData();
    NType* pB = B.getData();
    for(int i = 0; i < this->length; i++)
    {
        this->data[i] = pA[i] * pB[i];
    }
    return (*this);
}

template <typename NType>
NArray<NType>& NArray<NType>::mul(NArray<NType>& A, NMatrix<NType>& B, bool orient)
{
    int i, j, k;
    NType value = 0;
    NType* pA = A.getData();
    NType* pB = B.getData();
    int lenA = A.getLength();
    int sizeColumnB = B.getSizeColumn();

    if(lenA == B.getLenRow() && orient)
    {
        int lenB = B.getLenColumn();
        if(lenB > this->size) {this->resize(lenB);}
        this->length = lenB;

        for(j = 0; j < lenB; j++)
        {
            value = 0;
            for(k = 0; k < lenA; k++)
            {
                value += pA[k] * pB[k*sizeColumnB + j];
            }
            this->data[j] = value;
        }
    }
    else if(lenA == B.getLenColumn() && !orient)
    {
        int lenB = B.getLenRow();
        if(lenB > this->size) {this->resize(lenB);}
        this->length = lenB;

        for(i = 0; i < lenB; i++)
        {
            value = 0;
            for(k = 0; k < lenA; k++)
            {
                value += pA[k] * pB[i*sizeColumnB + k];
            }
            this->data[i] = value;
        }
    }
    return (*this);
}

template <typename NType>
NArray<NType>& NArray<NType>::mul(NMatrix<NType>& B, NArray<NType>& A, bool orient)
{
    return mul(A, B, orient);
}

#endif
