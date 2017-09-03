#ifndef NMATRIX_H
#define NMATRIX_H

#include "narray.h"
#include <mem.h>
#include <stdexcept>
#include <vector>

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
    NType* data; //Данные
    int lenRow; //Количество строк
    int lenColumn; //Количество столбцов
    int sizeRow; //Размерность строк
    int sizeColumn; //Размерность столбцов
public:
    int getLenRow();
    int getLenColumn();
    void setSize(int sizeRow, int sizeColumn);
    int getSizeRow();
    int getSizeColumn();
    void setData(NType* dt);
    NType* getData();
public:
    typedef NType* iterator;
    typedef const NType* const_iterator;
    NType* at(size_t pos); //Возвращает массив из указанной строки
    NType& front(); //Первый элемент матрицы
    NType& back(); //Последний элемент матрицы
    NMatrix<NType>::iterator begin() {return this->data;}
    NMatrix<NType>::iterator end() {return (this->data + this->sizeRow * this->sizeColumn);}
    NMatrix<NType>::const_iterator cbegin() const {return this->data;}
    NMatrix<NType>::const_iterator cend() const {return (this->data + this->sizeRow * this->sizeColumn);}
    NMatrix<NType>::iterator rbegin() {return (this->data + this->sizeRow * this->sizeColumn - 1);}
    NMatrix<NType>::iterator rend() {return this->data;}
    NMatrix<NType>::const_iterator crbegin() const {return (this->data + this->sizeRow * this->sizeColumn - 1);}
    NMatrix<NType>::const_iterator crend() const {return this->data;}
    bool empty(); //Матрица пустая?
    void shrink_to_fit(); //Очистка неиспользуемой памяти
    void swap(NMatrix<NType>& other); //Обмен значениями между матрицами
public:
    void init(int lenRow, int lenColumn); //Инициализация
    void init(int lenRow, int lenColumn, NType value); //Инициализация матрицы значением
    void clear(); //Очистка
    void addRow(int pos); //Добавление в матрицу новой строки
    void addRow(int pos, const NType &value); //Добавление в матрицу новой строки
    void delRow(int pos); //Удаление из матрицы строки
    void addColumn(int pos); //Добавление в матрицу новой колонки
    void addColumn(int pos, const NType &value); //Добавление в матрицу новой колонки
    void delColumn(int pos); //Удаление из матрицы колонки
    void pushRow(int count); //Добавление в матрицу указанного количества строк в конец
    void pushRow(std::vector<NType>& vec); //Добавление в матрицу вектора (строки)
    void pushRow(NArray<NType>& vec); //Добавление в матрицу вектора (строки)
    void popRow(int count); //Удаление из матрицы указанного количества строк с конца
    void popRow(std::vector<NType>& vec); //Удаление из матрицы вектора (строки) с конца
    void popRow(NArray<NType>& vec); //Удаление из матрицы вектора (строки) с конца
    void pushColumn(int count); //Добавление в матрицу указанного количества столбцов в конец
    void popColumn(int count); //Удаление из матрицы указанного количества колонок с конца
    void set(const NType &element, int posRow, int posColumn); //Установка элемента
    NType get(int posRow, int posColumn); //Возврат элемента
    void memCopy(NType* src, int oldSizeColumn); //Копирование данных матрицы
    void memCopy(NType* dst, NType* src, int newSizeColumn, int oldSizeColumn); //Копирование данных матрицы
    void dataCopy(NType* src, int oldSizeColumn); //Поэлементное копирование данных
    void dataCopy(NType* dst, NType* src, int newSizeColumn, int oldSizeColumn); //Поэлементное копирование данных
    void resize(int sizeRow, int sizeColumn); //Изменение размерности матрицы
    void newmin(); //Установка минимальной размерности
    void renew(int sizeRow, int sizeColumn); //Перевыделение памяти (все данные стираются)
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
    this->newmin();
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
void NMatrix<NType>::setSize(int sizeRow, int sizeColumn)
{
    this->sizeRow = sizeRow;
    this->sizeColumn = sizeColumn;
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
void NMatrix<NType>::setData(NType* dt)
{
    this->data = dt;
}

template <typename NType>
NType* NMatrix<NType>::getData()
{
    return this->data;
}


template <typename NType>
NType* NMatrix<NType>::at(size_t pos)
{
    if(pos >= (size_t)this->lenRow) {throw std::out_of_range("NMatrix<NType>::at() : index is out of range");}
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
void NMatrix<NType>::swap(NMatrix<NType>& other)
{
    NType* tmpdt = other.getData();
    other.setData(this->data);
    this->data = tmpdt;

    int tmpSizeRow = other.getSizeRow();
    int tmpSizeColumn = other.getSizeColumn();
    int tmpLenRow = other.getLenRow();
    int tmpLenColumn = other.getLenColumn();
    other.setSize(this->sizeRow, this->sizeColumn);
    other.init(this->lenRow, this->lenColumn);
    this->sizeRow = tmpSizeRow;
    this->sizeColumn = tmpSizeColumn;
    this->lenRow = tmpLenRow;
    this->lenColumn = tmpLenColumn;
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
void NMatrix<NType>::addRow(int pos)
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

    this->lenRow++;
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
void NMatrix<NType>::addColumn(int pos)
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

    this->lenColumn++;
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
void NMatrix<NType>::pushRow(int count)
{
    if(this->lenRow + count > this->sizeRow)
    {
        this->resize(this->lenRow + count, this->sizeColumn);
    }
    this->lenRow += count;
}

template <typename NType>
void NMatrix<NType>::pushRow(std::vector<NType>& vec)
{
    if(this->empty())
    {
        if(vec.size() > 0) {this->init(1, vec.size());}
        else {this->init(1, 0);}
    }
    else
    {
        int delta = vec.size() - this->lenColumn;
        if(delta > 0) {this->addColumn(delta);}
        this->pushRow(1);
    }

    int ind = this->lenRow - 1;
    for(size_t jnd = 0; jnd < vec.size(); jnd++)
    {
        this->data[ind*this->sizeColumn + jnd] = vec[jnd];
    }
}

template <typename NType>
void NMatrix<NType>::pushRow(NArray<NType>& vec)
{
    if(this->empty())
    {
        if(vec.getLength() > 0) {this->init(1, vec.getLength());}
        else {this->init(1, 0);}
    }
    else
    {
        int delta = vec.getLength() - this->lenColumn;
        if(delta > 0) {this->addColumn(delta);}
        this->pushRow(1);
    }

    int ind = this->lenRow - 1;
    for(int jnd = 0; jnd < vec.getLength(); jnd++)
    {
        this->data[ind*this->sizeColumn + jnd] = vec[jnd];
    }
}

template <typename NType>
void NMatrix<NType>::popRow(int count)
{
    this->lenRow -= count;
}

template <typename NType>
void NMatrix<NType>::popRow(std::vector<NType>& vec)
{
    int ind = this->lenRow - 1;
    vec.clear();
    for(int jnd = 0; jnd < this->lenColumn; jnd++)
    {
        vec.push_back(this->data[ind*this->sizeColumn + jnd]);
    }
    this->popRow(1);
}

template <typename NType>
void NMatrix<NType>::popRow(NArray<NType>& vec)
{
    int ind = this->lenRow - 1;
    vec.clear();
    for(int jnd = 0; jnd < this->lenColumn; jnd++)
    {
        vec.push(this->data[ind*this->sizeColumn + jnd]);
    }
    this->popRow(1);
}

template <typename NType>
void NMatrix<NType>::pushColumn(int count)
{
    if(this->lenColumn + count > this->sizeColumn)
    {
        this->resize(this->sizeRow, this->lenColumn + count);
    }
    this->lenColumn += count;
}

template <typename NType>
void NMatrix<NType>::popColumn(int count)
{
    this->lenColumn -= count;
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
void NMatrix<NType>::memCopy(NType* src, int oldSizeColumn)
{
    int m_column = this->lenColumn * sizeof(NType);
    NType* dataShift = this->data;
    NType* pShift = src;

    for(int i = 0; i < this->lenRow; i++)
    {
        memcpy(dataShift, pShift, m_column);
        dataShift += this->sizeColumn;
        pShift += oldSizeColumn;
    }
}

template <typename NType>
void NMatrix<NType>::memCopy(NType* dst, NType* src, int newSizeColumn, int oldSizeColumn)
{
    int m_column = this->lenColumn * sizeof(NType);
    NType* dataShift = dst;
    NType* pShift = src;

    for(int i = 0; i < this->lenRow; i++)
    {
        memcpy(dataShift, pShift, m_column);
        dataShift += newSizeColumn;
        pShift += oldSizeColumn;
    }
}

template <typename NType>
void NMatrix<NType>::dataCopy(NType* src, int oldSizeColumn)
{
    int i, j;
    for(i = 0; i < this->lenRow; i++)
    {
        for(j = 0; j < this->lenColumn; j++)
        {
            data[i*sizeColumn + j] = src[i*oldSizeColumn + j];
        }
    }
}

template <typename NType>
void NMatrix<NType>::dataCopy(NType* dst, NType* src, int newSizeColumn, int oldSizeColumn)
{
    int i, j;
    for(int i = 0; i < this->lenRow; i++)
    {
        for(j = 0; j < this->lenColumn; j++)
        {
            dst[i*newSizeColumn + j] = src[i*oldSizeColumn + j];
        }
    }
}

template <typename NType>
void NMatrix<NType>::resize(int sizeRow, int sizeColumn)
{
    sizeRow = sizeRow > this->lenRow ? sizeRow : this->lenRow;
    sizeColumn = sizeColumn > this->sizeColumn ? sizeColumn : this->sizeColumn;

    if((this->sizeRow != sizeRow)||(this->sizeColumn != sizeColumn))
    {
        NType* p = this->data;
        int num = sizeRow * sizeColumn;
        int oldSizeColumn = this->sizeColumn;

        if(num == 0)
        {
            this->newmin();
        }
        else
        {
            this->data = new NType[num];
            this->sizeRow = sizeRow;
            this->sizeColumn = sizeColumn;
        }

        if(p != nullptr)
        {
            this->dataCopy(p, oldSizeColumn);
            delete[] p;
        }
    }
}

template <typename NType>
void NMatrix<NType>::newmin()
{
    this->data = new NType[1];
    this->sizeRow = 1;
    this->sizeColumn = 1;
}

template <typename NType>
void NMatrix<NType>::renew(int sizeRow, int sizeColumn)
{
    delete[] this->data;
    int num = sizeRow * sizeColumn;
    if(num == 0)
    {
        this->newmin();
    }
    else
    {
        this->data = new NType[num];
        this->sizeRow = sizeRow;
        this->sizeColumn = sizeColumn;
    }
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
        int num = this->lenRow * this->sizeColumn;
        if(num == 0)
        {
            datacpy = new NType[1];
        }
        else
        {
            datacpy = new NType[num];
            this->memCopy(datacpy, this->data, this->sizeColumn, this->sizeColumn);
        }
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
