#ifndef NMATRIX_H
#define NMATRIX_H

#include "narray.h"
#include <mem.h>
#include <stdexcept>

template <typename NType> class NArray;

template <typename NType>
class NMatrix
{
public:
    NMatrix();
    NMatrix(NMatrix<NType> &obj);
    NMatrix<NType>& operator=(NMatrix<NType>& obj);
    ~NMatrix();
    NType* operator[](size_t pos);
protected:
    NType* data;
    int lenRow;
    int lenColumn;
    int sizeRow;
    int sizeColumn;
public:
    typedef NType* iterator;
    typedef const NType* const_iterator;
    NType* at(size_t pos);
    NType& front();
    NType& back();
    NMatrix<NType>::iterator begin() {return this->data;}
    NMatrix<NType>::iterator end() {return (this->data + this->sizeRow * this->sizeColumn);}
    NMatrix<NType>::const_iterator cbegin() const {return this->data;}
    NMatrix<NType>::const_iterator cend() const {return (this->data + this->sizeRow * this->sizeColumn);}
    NMatrix<NType>::iterator rbegin() {return (this->data + this->sizeRow * this->sizeColumn - 1);}
    NMatrix<NType>::iterator rend() {return this->data;}
    NMatrix<NType>::const_iterator crbegin() const {return (this->data + this->sizeRow * this->sizeColumn - 1);}
    NMatrix<NType>::const_iterator crend() const {return this->data;}
    bool empty();
    void shrink_to_fit();
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
    void dataCopy(NType* src, int oldSizeColumn);
    void dataCopy(NType* dst, NType* src, int newSizeColumn, int oldSizeColumn);
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


template <typename NType>
NMatrix<NType>::NMatrix()
{
    this->data = nullptr;
    this->lenRow = 0;
    this->lenColumn = 0;
    this->sizeRow = 0;
    this->sizeColumn = 0;
}

template <typename NType>
NMatrix<NType>::NMatrix(NMatrix<NType> &obj)
{
    this->lenRow = obj.getLenRow();
    this->lenColumn = obj.getLenColumn();
    this->sizeRow = obj.getSizeRow();
    this->sizeColumn = obj.getSizeColumn();
    this->data = new NType[this->sizeRow * this->sizeColumn];

    NType* p = obj.getData();
    if(p != nullptr)
    {
        this->dataCopy(p, this->sizeColumn);
    }
}

template <typename NType>
NMatrix<NType>& NMatrix<NType>::operator=(NMatrix<NType>& obj)
{
    int lenRow = obj.getLenRow();
    int lenColumn = obj.getLenColumn();

    if((lenRow > this->sizeRow)||(lenColumn > this->sizeColumn))
    {
        this->renew(lenRow, lenColumn);
    }
    this->lenRow = lenRow;
    this->lenColumn = lenColumn;

    NType* p = obj.getData();
    if(p != nullptr)
    {
        this->dataCopy(p, obj.getSizeColumn());
    }

    return *this;
}

template <typename NType>
NMatrix<NType>::~NMatrix()
{
    if(this->data != nullptr) {delete[] this->data;}
}

template <typename NType>
NType* NMatrix<NType>::operator[](size_t pos)
{
    return (this->data + (pos*this->sizeColumn));
}


template <typename NType>
NType* NMatrix<NType>::at(size_t pos)
{
    if(pos >= this->lenRow) {throw std::out_of_range("NMatrix<NType>::at() : index is out of range");}
    return (this->data + (pos * this->sizeColumn));
}

template <typename NType>
NType& NMatrix<NType>::front()
{
    return this->data[0];
}

template <typename NType>
NType& NMatrix<NType>::back()
{
    return this->data[(this->lenRow-1) * this->sizeColumn + (this->lenColumn-1)];
}

template <typename NType>
bool NMatrix<NType>::empty()
{
    return (this->lenRow == 0 || this->lenColumn == 0);
}

template <typename NType>
void NMatrix<NType>::shrink_to_fit()
{
    this->resize(this->lenRow, this->lenColumn);
}


template <typename NType>
void NMatrix<NType>::init(int lenRow, int lenColumn)
{
    if((lenRow > this->sizeRow)||(lenColumn > this->sizeColumn))
    {
        this->resize(lenRow, lenColumn);
    }

    this->lenRow = lenRow;
    this->lenColumn = lenColumn;
}

template <typename NType>
void NMatrix<NType>::init(int lenRow, int lenColumn, NType value)
{
    if((lenRow > this->sizeRow)||(lenColumn > this->sizeColumn))
    {
        //this->resize(lenRow, lenColumn);
        this->renew(lenRow, lenColumn);
    }

    this->lenRow = lenRow;
    this->lenColumn = lenColumn;
    for(int i = 0; i < lenRow * lenColumn; i++)
    {
        this->data[i] = value;
    }
}

template <typename NType>
void NMatrix<NType>::clear()
{
    this->lenRow = 0;
    this->lenColumn = 0;
}

template <typename NType>
void NMatrix<NType>::addRow(int pos, const NType &value)
{
    int i, j;
    if(this->lenRow >= this->sizeRow)
    {
        this->resize(this->lenRow + 1, this->sizeColumn);
    }

    for(i = this->lenRow; i > pos; i--)
    {
        for(j = 0; j < this->lenColumn; j++)
        {
            this->data[i*this->sizeColumn + j] = this->data[(i-1)*this->sizeColumn + j];
        }
    }
    for(j = 0; j < this->lenColumn; j++)
    {
        this->data[pos*this->sizeColumn + j] = value;
    }
    this->lenRow++;
}

template <typename NType>
void NMatrix<NType>::delRow(int pos)
{
    int i, j;
    this->lenRow--;
    for(i = pos; i < this->lenRow; i++)
    {
        for(j = 0; j < this->lenColumn; j++)
        {
            this->data[i*this->sizeColumn + j] = this->data[(i+1)*this->sizeColumn + j];
        }
    }
}

template <typename NType>
void NMatrix<NType>::addColumn(int pos, const NType &value)
{
    int i, j;
    if(this->lenColumn >= this->sizeColumn)
    {
        this->resize(this->sizeRow, this->lenColumn + 1);
    }

    for(i = 0; i < this->lenRow; i++)
    {
        for(j = this->lenColumn; j > pos; j--)
        {
            this->data[i*this->sizeColumn + j] = this->data[i*this->sizeColumn + (j-1)];
        }
    }
    for(i = 0; i < this->lenRow; i++)
    {
        this->data[i*this->sizeColumn + pos] = value;
    }
    this->lenColumn++;
}

template <typename NType>
void NMatrix<NType>::delColumn(int pos)
{
    int i, j;
    this->lenColumn--;
    for(i = 0; i < this->lenRow; i++)
    {
        for(j = pos; j < this->lenColumn; j++)
        {
            this->data[i*this->sizeColumn + j] = this->data[i*this->sizeColumn + (j+1)];
        }
    }
}

template <typename NType>
void NMatrix<NType>::set(const NType &element, int posRow, int posColumn)
{
    this->data[posRow*this->sizeColumn + posColumn] = element;
}

template <typename NType>
NType NMatrix<NType>::get(int posRow, int posColumn)
{
    return this->data[posRow*this->sizeColumn + posColumn];
}

template <typename NType>
NType* NMatrix<NType>::getData()
{
    return this->data;
}

template <typename NType>
void NMatrix<NType>::dataCopy(NType* src, int oldSizeColumn)
{
    //int i, j;
    int m_column = this->lenColumn * sizeof(NType);
    NType* dataShift = this->data;
    NType* pShift = src;

    for(int i = 0; i < this->lenRow; i++)
    {
        memcpy(dataShift, pShift, m_column);
        dataShift += this->sizeColumn;
        pShift += oldSizeColumn;
        /*for(j = 0; j < this->lenColumn; j++)
        {
            dst[i*newSizeColumn + j] = src[i*oldSizeColumn + j];
        }*/
    }
}

template <typename NType>
void NMatrix<NType>::dataCopy(NType* dst, NType* src, int newSizeColumn, int oldSizeColumn)
{
    //int i, j;
    int m_column = this->lenColumn * sizeof(NType);
    NType* dataShift = dst;
    NType* pShift = src;

    for(int i = 0; i < this->lenRow; i++)
    {
        memcpy(dataShift, pShift, m_column);
        dataShift += newSizeColumn;
        pShift += oldSizeColumn;
        /*for(j = 0; j < this->lenColumn; j++)
        {
            dst[i*newSizeColumn + j] = src[i*oldSizeColumn + j];
        }*/
    }
}

template <typename NType>
void NMatrix<NType>::resize(int sizeRow, int sizeColumn)
{
    sizeRow = sizeRow > this->lenRow ? sizeRow : this->lenRow;
    sizeColumn = sizeColumn > this->sizeColumn ? sizeColumn : this->sizeColumn;

    if((this->sizeRow != sizeRow)&&(this->sizeColumn != sizeColumn))
    {
        NType* p = this->data;
        this->data = new NType[sizeRow * sizeColumn];
        int oldSizeColumn = this->sizeColumn;
        this->sizeRow = sizeRow;
        this->sizeColumn = sizeColumn;
        if(p != nullptr)
        {
            this->dataCopy(p, oldSizeColumn);
            delete[] p;
        }
    }
}

template <typename NType>
void NMatrix<NType>::renew(int sizeRow, int sizeColumn)
{
    delete[] this->data;
    this->data = new NType[sizeRow * sizeColumn];
    this->sizeRow = sizeRow;
    this->sizeColumn = sizeColumn;
}

template <typename NType>
int NMatrix<NType>::getLenRow()
{
    return this->lenRow;
}

template <typename NType>
int NMatrix<NType>::getLenColumn()
{
    return this->lenColumn;
}

template <typename NType>
int NMatrix<NType>::getSizeRow()
{
    return this->sizeRow;
}

template <typename NType>
int NMatrix<NType>::getSizeColumn()
{
    return this->sizeColumn;
}


template <typename NType>
NMatrix<NType>& NMatrix<NType>::sum(NMatrix<NType>& B)
{
    int i, j;
    NType* pB = B.getData();
    for(i = 0; i < lenRow; i++)
    {
        for(j = 0; j < lenColumn; j++)
        {
            data[i*sizeColumn + j] += pB[i*sizeColumn + j];
        }
    }
    return (*this);
}

template <typename NType>
NMatrix<NType>& NMatrix<NType>::sum(NMatrix<NType>& A, NMatrix<NType>& B)
{
    int i, j;
    NType* pA = A.getData();
    NType* pB = B.getData();

    for(i = 0; i < lenRow; i++)
    {
        for(j = 0; j < lenColumn; j++)
        {
            data[i*sizeColumn + j] = pA[i*sizeColumn + j] + pB[i*sizeColumn + j];
        }
    }
    return (*this);
}

template <typename NType>
NMatrix<NType>& NMatrix<NType>::valsum(NType& B)
{
    int i, j;
    for(i = 0; i < lenRow; i++)
    {
        for(j = 0; j < lenColumn; j++)
        {
            data[i*sizeColumn + j] += B;
        }
    }
    return (*this);
}

template <typename NType>
NMatrix<NType>& NMatrix<NType>::mul(NArray<NType>& A, NArray<NType>& B)
{
    int i, j;
    NType* pA = A.getData();
    NType* pB = B.getData();
    int lenA = A.getLength();
    int lenB = B.getLength();

    this->init(lenA, lenB);
    for(i = 0; i < lenRow; i++)
    {
        for(j = 0; j < lenColumn; j++)
        {
            data[i*sizeColumn + j] = pA[i] * pB[j];
        }
    }
    return (*this);
}

template <typename NType>
NMatrix<NType>& NMatrix<NType>::mul(NMatrix<NType>& B)
{
    //return this->mul(*this, B);
    NType* datacpy = nullptr;
    if(this->data != nullptr)
    {
        datacpy = new NType[this->lenRow * this->sizeColumn];
        this->dataCopy(datacpy, this->data, this->sizeColumn, this->sizeColumn);
    }

    int i, j, k;
    NType value = 0;
    NType* pB = B.getData();
    int sizeColumnA = this->getSizeColumn();
    int sizeColumnB = B.getSizeColumn();
    int lenRowA = this->getLenRow();
    int lenColumnA = this->getLenColumn();

    if(lenRowA == B.getLenColumn())
    {
        this->init(B.getLenRow(), this->getLenColumn(), value);
        for(i = 0; i < lenRow; i++)
        {
            for(j = 0; j < lenColumn; j++)
            {
                value = 0;
                for(k = 0; k < lenRowA; k++)
                {
                    value += datacpy[k*sizeColumnA + j] * pB[i*sizeColumnB + k];
                }
                data[i*sizeColumn + j] = value;
            }
        }
    }
    else
    {
        this->init(this->getLenRow(), B.getLenColumn(), value);
        for(i = 0; i < lenRow; i++)
        {
            for(j = 0; j < lenColumn; j++)
            {
                value = 0;
                for(k = 0; k < lenColumnA; k++)
                {
                    value += datacpy[i*sizeColumnA + k] * pB[k*sizeColumnB + j];
                }
                data[i*sizeColumn + j] = value;
            }
        }
    }
    return (*this);
}

template <typename NType>
NMatrix<NType>& NMatrix<NType>::mul(NMatrix<NType>& A, NMatrix<NType>& B)
{
    int i, j, k;
    NType value = 0;
    NType* pA = A.getData();
    NType* pB = B.getData();
    int sizeColumnA = A.getSizeColumn();
    int sizeColumnB = B.getSizeColumn();
    int lenRowA = A.getLenRow();
    int lenColumnA = A.getLenColumn();

    if(lenRowA == B.getLenColumn())
    {
        this->init(B.getLenRow(), A.getLenColumn(), value);
        for(i = 0; i < lenRow; i++)
        {
            for(j = 0; j < lenColumn; j++)
            {
                value = 0;
                for(k = 0; k < lenRowA; k++)
                {
                    value += pA[k*sizeColumnA + j] * pB[i*sizeColumnB + k];
                }
                data[i*sizeColumn + j] = value;
            }
        }
    }
    else
    {
        this->init(A.getLenRow(), B.getLenColumn(), value);
        for(i = 0; i < lenRow; i++)
        {
            for(j = 0; j < lenColumn; j++)
            {
                value = 0;
                for(k = 0; k < lenColumnA; k++)
                {
                    value += pA[i*sizeColumnA + k] * pB[k*sizeColumnB + j];
                }
                data[i*sizeColumn + j] = value;
            }
        }
    }
    return (*this);
}

template <typename NType>
NMatrix<NType>& NMatrix<NType>::matmul(NMatrix<NType>& B)
{
    int i, j;
    NType* pB = B.getData();
    for(i = 0; i < lenRow; i++)
    {
        for(j = 0; j < lenColumn; j++)
        {
            data[i*sizeColumn + j] *= pB[i*sizeColumn + j];
        }
    }
    return (*this);
}

template <typename NType>
NMatrix<NType>& NMatrix<NType>::valmul(NType& B)
{
    int i, j;
    for(i = 0; i < lenRow; i++)
    {
        for(j = 0; j < lenColumn; j++)
        {
            data[i*sizeColumn + j] *= B;
        }
    }
    return (*this);
}

#endif // NMATRIX_H
