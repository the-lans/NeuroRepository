#ifndef NARRAY_H
#define NARRAY_H

#include "nmatrix.h"
#include <mem.h>
#include <stdexcept>

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
    NType& operator[](size_t pos);
    const NType& operator[](size_t pos) const;
protected:
    int length;
    int size;
    int block;
    NType* data;
public:
    typedef NType* iterator;
    typedef const NType* const_iterator;
    NType& at(size_t pos);
    NType& front();
    NType& back();
    NArray<NType>::iterator begin() {return this->data;}
    NArray<NType>::iterator end() {return (this->data + this->length);}
    NArray<NType>::const_iterator cbegin() const {return this->data;}
    NArray<NType>::const_iterator cend() const {return (this->data + this->length);}
    NArray<NType>::iterator rbegin() {return (this->data + this->length - 1);}
    NArray<NType>::iterator rend() {return this->data;}
    NArray<NType>::const_iterator crbegin() const {return (this->data + this->length - 1);}
    NArray<NType>::const_iterator crend() const {return this->data;}
    bool empty();
    void shrink_to_fit();
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
    void dataCopy(NType* src);
    void dataCopy(NType* dst, NType* src);
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
    if(p != nullptr)
    {
        this->dataCopy(p);
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
    if(p != nullptr)
    {
        this->dataCopy(p);
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
NType& NArray<NType>::operator[](size_t pos)
{
    return this->data[pos];
}

template <typename NType>
const NType& NArray<NType>::operator[](size_t pos) const
{
    return this->data[pos];
}


template <typename NType>
NType& NArray<NType>::at(size_t pos)
{
    if(pos >= this->length) {throw std::out_of_range("NArray<NType>::at() : index is out of range");}
    return this->data[pos];
}

template <typename NType>
NType& NArray<NType>::front()
{
    return this->data[0];
}

template <typename NType>
NType& NArray<NType>::back()
{
    return this->data[this->length-1];
}

template <typename NType>
bool NArray<NType>::empty()
{
    return (this->length == 0);
}

template <typename NType>
void NArray<NType>::shrink_to_fit()
{
    this->resize(this->length);
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
        if(p != nullptr)
        {
            this->dataCopy(p);
            delete[] p;
        }
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
void NArray<NType>::dataCopy(NType* src)
{
    memcpy(this->data, src, this->length * sizeof(NType));
    /*for(int i = 0; i < this->length; i++)
    {
        this->data[i] = src[i];
    }*/
}

template <typename NType>
void NArray<NType>::dataCopy(NType* dst, NType* src)
{
    memcpy(dst, src, this->length * sizeof(NType));
    /*for(int i = 0; i < this->length; i++)
    {
        dst[i] = src[i];
    }*/
}


template <typename NType>
NArray<NType>& NArray<NType>::valsum(NType& B)
{
    for(int i = 0; i < length; i++)
    {
        data[i] += B;
    }
    return (*this);
}

template <typename NType>
NArray<NType>& NArray<NType>::sum(NArray<NType>& B)
{
    NType* pB = B.getData();
    for(int i = 0; i < length; i++)
    {
        data[i] += pB[i];
    }
    return (*this);
}

template <typename NType>
NArray<NType>& NArray<NType>::sum(NArray<NType>& A, NArray<NType>& B)
{
    NType* pA = A.getData();
    NType* pB = B.getData();
    for(int i = 0; i < length; i++)
    {
        data[i] = pA[i] + pB[i];
    }
    return (*this);
}

template <typename NType>
NArray<NType>& NArray<NType>::valmul(NType& B)
{
    for(int i = 0; i < length; i++)
    {
        data[i] *= B;
    }
    return (*this);
}

template <typename NType>
NArray<NType>& NArray<NType>::mul(NMatrix<NType>& B, bool orient)
{
    //return this->mul(*this, B, orient);
    NType* datacpy = nullptr;
    if(data != nullptr)
    {
        datacpy = new NType[this->length];
        this->dataCopy(datacpy, this->data);
    }

    int i, j, k;
    NType value = 0;
    NType* pB = B.getData();
    int lenA = length;
    int sizeColumnB = B.getSizeColumn();

    if(lenA == B.getLenRow() && orient)
    {
        int lenB = B.getLenColumn();
        if(lenB > size) {resize(lenB);}
        length = lenB;

        for(j = 0; j < lenB; j++)
        {
            value = 0;
            for(k = 0; k < lenA; k++)
            {
                value += datacpy[k] * pB[k*sizeColumnB + j];
            }
            data[j] = value;
        }
    }
    else if(lenA == B.getLenColumn() && !orient)
    {
        int lenB = B.getLenRow();
        if(lenB > size) {resize(lenB);}
        length = lenB;

        for(i = 0; i < lenB; i++)
        {
            value = 0;
            for(k = 0; k < lenA; k++)
            {
                value += datacpy[k] * pB[i*sizeColumnB + k];
            }
            data[i] = value;
        }
    }
    return (*this);
}

template <typename NType>
NArray<NType>& NArray<NType>::mul(NArray<NType>& B)
{
    //return this->mul(*this, B);
    NType* pB = B.getData();
    for(int i = 0; i < length; i++)
    {
        data[i] *= pB[i];
    }
    return (*this);
}

template <typename NType>
NArray<NType>& NArray<NType>::mul(NArray<NType>& A, NArray<NType>& B)
{
    NType* pA = A.getData();
    NType* pB = B.getData();
    for(int i = 0; i < length; i++)
    {
        data[i] = pA[i] * pB[i];
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
        if(lenB > size) {resize(lenB);}
        length = lenB;

        for(j = 0; j < lenB; j++)
        {
            value = 0;
            for(k = 0; k < lenA; k++)
            {
                value += pA[k] * pB[k*sizeColumnB + j];
            }
            data[j] = value;
        }
    }
    else if(lenA == B.getLenColumn() && !orient)
    {
        int lenB = B.getLenRow();
        if(lenB > size) {resize(lenB);}
        length = lenB;

        for(i = 0; i < lenB; i++)
        {
            value = 0;
            for(k = 0; k < lenA; k++)
            {
                value += pA[k] * pB[i*sizeColumnB + k];
            }
            data[i] = value;
        }
    }
    return (*this);
}

template <typename NType>
NArray<NType>& NArray<NType>::mul(NMatrix<NType>& B, NArray<NType>& A, bool orient)
{
    return this->mul(A, B, orient);
}

#endif // NARRAY_H
