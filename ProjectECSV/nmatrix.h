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
    bool lock; //Запрет на изменение размерности массива
    bool extrn; //В матрице внешние данные
public:
    int getLenRow();
    int getLenColumn();
    void setLength(int sizeRow, int sizeColumn);
    void setSize(int sizeRow, int sizeColumn);
    int getSize();
    int getSizeRow();
    int getSizeColumn();
    void setData(NType* dt);
    NType* getData();
    void setLock(bool bl);
    bool getLock();
    void setExtrn(bool bl);
    bool getExtrn();
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
    void copyValue(NMatrix<NType>& obj); //Копирование значений
    void destruct(); //Деструктор
    void init(int lenRow, int lenColumn); //Инициализация
    void init(int lenRow, int lenColumn, const NType& value); //Инициализация матрицы значением
    void init_value(const NType& value); //Инициализация матрицы значением
    void init_rand(std::default_random_engine& generator, const NType& valMin, const NType& valMax); //Инициализация случайными числами
    void clear(); //Очистка
    void addRow(int pos); //Добавление в матрицу новой строки
    void addRow(int pos, const NType &value);
    void addRow(int pos, std::vector<NType>& vec);
    void addRow(int pos, NArray<NType>& vec);
    void delRow(int pos); //Удаление из матрицы строки
    void delRow(int pos, std::vector<NType>& vec);
    void delRow(int pos, NArray<NType>& vec);
    void addColumn(int pos); //Добавление в матрицу новой колонки
    void addColumn(int pos, const NType &value); //Добавление в матрицу новой колонки
    void addColumn(int pos, std::vector<NType>& vec);
    void addColumn(int pos, NArray<NType>& vec);
    void delColumn(int pos); //Удаление из матрицы колонки
    void delColumn(int pos, std::vector<NType>& vec);
    void delColumn(int pos, NArray<NType>& vec);
    void pushRow(int count); //Добавление в матрицу указанного количества строк в конец
    void pushRow(std::vector<NType>& vec); //Добавление в матрицу вектора (строки)
    void pushRow(NArray<NType>& vec); //Добавление в матрицу вектора (строки)
    void popRow(int count); //Удаление из матрицы указанного количества строк с конца
    void popRow(std::vector<NType>& vec); //Удаление из матрицы вектора (строки) с конца
    void popRow(NArray<NType>& vec); //Удаление из матрицы вектора (строки) с конца
    void pushColumn(int count); //Добавление в матрицу указанного количества столбцов в конец
    void pushColumn(std::vector<NType>& vec); //Добавление в матрицу вектора (столбца)
    void pushColumn(NArray<NType>& vec); //Добавление в матрицу вектора (столбца)
    void popColumn(int count); //Удаление из матрицы указанного количества колонок с конца
    void popColumn(std::vector<NType>& vec); //Удаление из матрицы вектора (столбца) с конца
    void popColumn(NArray<NType>& vec); //Удаление из матрицы вектора (столбца) с конца
    void setRow(int pos, std::vector<NType>& vec); //Установить строку
    void setRow(int pos, NArray<NType>& vec);
    void getRow(int pos, std::vector<NType>& vec); //Вернуть строку
    void getRow(int pos, NArray<NType>& vec);
    void setColumn(int pos, std::vector<NType>& vec); //Установить колонку
    void setColumn(int pos, NArray<NType>& vec);
    void getColumn(int pos, std::vector<NType>& vec); //Вернуть колонку
    void getColumn(int pos, NArray<NType>& vec);
    void set(const NType &element, int posRow, int posColumn); //Установка элемента
    NType get(int posRow, int posColumn); //Возврат элемента
    void memCopy(NType* src, int oldSizeColumn); //Копирование данных матрицы
    void memCopy(NType* dst, NType* src, int newSizeColumn, int oldSizeColumn); //Копирование данных матрицы
    void dataCopy(NType* src, int oldSizeColumn); //Поэлементное копирование данных
    void dataCopy(NType* dst, NType* src, int newSizeColumn, int oldSizeColumn); //Поэлементное копирование данных
    void resize(int sizeRow, int sizeColumn); //Изменение размерности матрицы
    void newmin(); //Установка минимальной размерности
    void renew(); //Перевыделение памяти (все данные стираются)
    void renew(int sizeRow, int sizeColumn); //Перевыделение памяти (все данные стираются)
    void copyFields(NMatrix<NType>& obj); //Копирование полей
    void doMaskRow(bool* mask); //Обнуление элементов по маске
    void doMaskColumn(bool* mask); //Обнуление элементов по маске
public:
    NMatrix<NType>& sum(NMatrix<NType>& B);
    NMatrix<NType>& sum(NMatrix<NType>& A, NMatrix<NType>& B);
    NMatrix<NType>& valsum(const NType& B);
    NMatrix<NType>& mul(NArray<NType>& A, NArray<NType>& B);
    NMatrix<NType>& mul(NMatrix<NType>& B, bool orient);
    NMatrix<NType>& mul(NMatrix<NType>& A, NMatrix<NType>& B, bool orient);
    NMatrix<NType>& matmul(NMatrix<NType>& B);
    NMatrix<NType>& valmul(const NType& B);
    NMatrix<NType>& valsign();
};


template <typename NType>
NMatrix<NType>::NMatrix()
{
    this->data = nullptr;
    this->lenRow = 0;
    this->lenColumn = 0;
    this->sizeRow = 0;
    this->sizeColumn = 0;
    this->lock = false;
    this->extrn = false;
    //this->newmin();
}

template <typename NType>
NMatrix<NType>::NMatrix(NMatrix<NType> &obj)
{
    this->lenRow = obj.getLenRow();
    this->lenColumn = obj.getLenColumn();
    this->sizeRow = obj.getSizeRow();
    this->sizeColumn = obj.getSizeColumn();
    this->extrn = false;

    this->data = new NType[this->sizeRow * this->sizeColumn];

    NType* p = obj.getData();
    if(p != nullptr) {this->dataCopy(p, this->sizeColumn);}
    //this->lock = obj.getLock();
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
    //this->extrn = false;

    NType* p = obj.getData();
    if(p != nullptr) {this->dataCopy(p, obj.getSizeColumn());}
    //this->lock = obj.getLock();

    return *this;
}

template <typename NType>
NMatrix<NType>::~NMatrix()
{
    this->destruct();
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
void NMatrix<NType>::setLength(int sizeRow, int sizeColumn)
{
    this->lenRow = sizeRow;
    this->lenColumn = sizeColumn;
}

template <typename NType>
void NMatrix<NType>::setSize(int sizeRow, int sizeColumn)
{
    this->sizeRow = sizeRow;
    this->sizeColumn = sizeColumn;
}

template <typename NType>
int NMatrix<NType>::getSize()
{
    return this->sizeRow * this->sizeColumn;
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
void NMatrix<NType>::setLock(bool bl)
{
    this->lock = bl;
}

template <typename NType>
bool NMatrix<NType>::getLock()
{
    return this->lock;
}

template <typename NType>
void NMatrix<NType>::setExtrn(bool bl)
{
    this->extrn = bl;
}

template <typename NType>
bool NMatrix<NType>::getExtrn()
{
    return this->extrn;
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
    return (this->lenRow == 0 && this->lenColumn == 0);
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
    bool tmpLock = other.getLock();
    bool tmpExtrn = other.getExtrn();
    other.setSize(this->sizeRow, this->sizeColumn);
    other.setLength(this->lenRow, this->lenColumn);
    other.setLock(this->lock);
    other.setExtrn(this->extrn);
    this->sizeRow = tmpSizeRow;
    this->sizeColumn = tmpSizeColumn;
    this->lenRow = tmpLenRow;
    this->lenColumn = tmpLenColumn;
    this->lock = tmpLock;
    this->extrn = tmpExtrn;
}


template <typename NType>
void NMatrix<NType>::copyValue(NMatrix<NType>& obj)
{
    int lenRow = obj.getLenRow();
    int lenColumn = obj.getLenColumn();

    if((lenRow > this->sizeRow)||(lenColumn > this->sizeColumn))
    {
        this->renew(lenRow, lenColumn);
    }
    this->lenRow = lenRow;
    this->lenColumn = lenColumn;
    //this->extrn = false;

    NType* p = obj.getData();
    if(p != nullptr) {this->memCopy(p, obj.getSizeColumn());}
    //this->lock = obj.getLock();
}

template <typename NType>
void NMatrix<NType>::destruct()
{
    if(this->data != nullptr && !this->extrn)
    {
        delete[] this->data;
        this->data = nullptr;
    }
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
void NMatrix<NType>::init(int lenRow, int lenColumn, const NType& value)
{
    if((lenRow > this->sizeRow)||(lenColumn > this->sizeColumn))
    {
        //this->resize(lenRow, lenColumn);
        this->renew(lenRow, lenColumn);
    }

    this->lenRow = lenRow;
    this->lenColumn = lenColumn;
    this->init_value(value);
}

template <typename NType>
void NMatrix<NType>::init_value(const NType& value)
{
    for(int i = 0; i < lenRow * lenColumn; i++)
    {
        data[i] = value;
    }
}

template <typename NType>
void NMatrix<NType>::init_rand(std::default_random_engine& generator, const NType& valMin, const NType& valMax)
{
    std::uniform_real_distribution<> distribution(valMin, valMax);
    //NType koef = (valMax - valMin)/(NType)RAND_MAX;
    for(int i = 0; i < lenRow * lenColumn; i++)
    {
        //data[i] = koef * (NType)rand() + valMin;
        data[i] = distribution(generator);
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
void NMatrix<NType>::addRow(int pos, std::vector<NType>& vec)
{
    if(this->empty()) {this->init(1, vec.size());}
    else {this->addRow(pos);}
    this->setRow(pos, vec);
}

template <typename NType>
void NMatrix<NType>::addRow(int pos, NArray<NType>& vec)
{
    if(this->empty()) {this->init(1, vec.getLength());}
    else {this->addRow(pos);}
    this->setRow(pos, vec);
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
void NMatrix<NType>::delRow(int pos, std::vector<NType>& vec)
{
    this->getRow(pos, vec);
    this->delRow(pos);
}

template <typename NType>
void NMatrix<NType>::delRow(int pos, NArray<NType>& vec)
{
    this->getRow(pos, vec);
    this->delRow(pos);
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
void NMatrix<NType>::addColumn(int pos, std::vector<NType>& vec)
{
    if(this->empty()) {this->init(vec.size(), 1);}
    else {this->addColumn(pos);}
    this->setColumn(pos, vec);
}

template <typename NType>
void NMatrix<NType>::addColumn(int pos, NArray<NType>& vec)
{
    if(this->empty()) {this->init(vec.getLength(), 1);}
    else {this->addColumn(pos);}
    this->setColumn(pos, vec);
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
void NMatrix<NType>::delColumn(int pos, std::vector<NType>& vec)
{
    this->getColumn(pos, vec);
    this->delColumn(pos);
}

template <typename NType>
void NMatrix<NType>::delColumn(int pos, NArray<NType>& vec)
{
    this->getColumn(pos, vec);
    this->delColumn(pos);
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
    if(this->empty()) {this->init(1, vec.size());}
    else {this->pushRow(1);}
    this->setRow(this->lenRow - 1, vec);
}

template <typename NType>
void NMatrix<NType>::pushRow(NArray<NType>& vec)
{
    if(this->empty()) {this->init(1, vec.getLength());}
    else {this->pushRow(1);}
    this->setRow(this->lenRow - 1, vec);
}

template <typename NType>
void NMatrix<NType>::popRow(int count)
{
    if(count > this->lenRow) {this->lenRow = 0;}
    else {this->lenRow -= count;}
}

template <typename NType>
void NMatrix<NType>::popRow(std::vector<NType>& vec)
{
    if(this->lenRow <= 0) {throw std::out_of_range("NMatrix<NType>::popRow() : length < 0");}
    this->getRow(this->lenRow - 1, vec);
    this->popRow(1);
}

template <typename NType>
void NMatrix<NType>::popRow(NArray<NType>& vec)
{
    if(this->lenRow <= 0) {throw std::out_of_range("NMatrix<NType>::popRow() : length < 0");}
    this->getRow(this->lenRow - 1, vec);
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
void NMatrix<NType>::pushColumn(std::vector<NType>& vec)
{
    if(this->lenColumn == 0) {this->init(vec.size(), 1);}
    else {this->pushColumn(1);}
    this->setColumn(this->lenColumn - 1, vec);
}

template <typename NType>
void NMatrix<NType>::pushColumn(NArray<NType>& vec)
{
    if(this->lenColumn == 0) {this->init(vec.getLength(), 1);}
    else {this->pushColumn(1);}
    this->setColumn(this->lenColumn - 1, vec);
}

template <typename NType>
void NMatrix<NType>::popColumn(int count)
{
    if(count > this->lenColumn) {this->lenColumn = 0;}
    else {this->lenColumn -= count;}
}

template <typename NType>
void NMatrix<NType>::popColumn(std::vector<NType>& vec)
{
    if(this->lenColumn <= 0) {throw std::out_of_range("NMatrix<NType>::popColumn() : length < 0");}
    this->getColumn(this->lenColumn - 1, vec);
    this->popColumn(1);
}

template <typename NType>
void NMatrix<NType>::popColumn(NArray<NType>& vec)
{
    if(this->lenColumn <= 0) {throw std::out_of_range("NMatrix<NType>::popColumn() : length < 0");}
    this->getColumn(this->lenColumn - 1, vec);
    this->popColumn(1);
}

template <typename NType>
void NMatrix<NType>::setRow(int pos, std::vector<NType>& vec)
{
    if(pos >= this->lenRow) {throw std::out_of_range("NMatrix<NType>::setRow() : index is out of range");}
    if(vec.size() != (size_t)this->lenColumn) {throw std::out_of_range("NMatrix<NType>::setRow() : not length vector");}

    for(int j = 0; j < this->lenColumn; j++)
    {
        this->data[pos*this->sizeColumn + j] = vec[j];
    }
}

template <typename NType>
void NMatrix<NType>::setRow(int pos, NArray<NType>& vec)
{
    if(pos >= this->lenRow) {throw std::out_of_range("NMatrix<NType>::setRow() : index is out of range");}
    if(vec.getLength() != this->lenColumn) {throw std::out_of_range("NMatrix<NType>::setRow() : not length vector");}

    for(int j = 0; j < this->lenColumn; j++)
    {
        this->data[pos*this->sizeColumn + j] = vec[j];
    }
}

template <typename NType>
void NMatrix<NType>::getRow(int pos, std::vector<NType>& vec)
{
    if(pos >= this->lenRow) {throw std::out_of_range("NMatrix<NType>::getRow() : index is out of range");}

    vec.clear();
    for(int j = 0; j < this->lenColumn; j++)
    {
        vec.push_back(this->data[pos*this->sizeColumn + j]);
    }
}

template <typename NType>
void NMatrix<NType>::getRow(int pos, NArray<NType>& vec)
{
    if(pos >= this->lenRow) {throw std::out_of_range("NMatrix<NType>::getRow() : index is out of range");}

    vec.init(this->lenColumn);
    for(int j = 0; j < this->lenColumn; j++)
    {
        vec[j] = this->data[pos*this->sizeColumn + j];
    }
}

template <typename NType>
void NMatrix<NType>::setColumn(int pos, std::vector<NType>& vec)
{
    if(pos >= this->lenColumn) {throw std::out_of_range("NMatrix<NType>::setColumn() : index is out of range");}
    if(vec.size() != this->lenRow) {throw std::out_of_range("NMatrix<NType>::setColumn() : not length vector");}

    for(int i = 0; i < this->lenRow; i++)
    {
        this->data[i*this->sizeColumn + pos] = vec[i];
    }
}

template <typename NType>
void NMatrix<NType>::setColumn(int pos, NArray<NType>& vec)
{
    if(pos >= this->lenColumn) {throw std::out_of_range("NMatrix<NType>::setColumn() : index is out of range");}
    if(vec.getLength() != this->lenRow) {throw std::out_of_range("NMatrix<NType>::setColumn() : not length vector");}

    for(int i = 0; i < this->lenRow; i++)
    {
        this->data[i*this->sizeColumn + pos] = vec[i];
    }
}

template <typename NType>
void NMatrix<NType>::getColumn(int pos, std::vector<NType>& vec)
{
    if(pos >= this->lenColumn) {throw std::out_of_range("NMatrix<NType>::getColumn() : index is out of range");}

    vec.clear();
    for(int i = 0; i < this->lenRow; i++)
    {
        vec.push_back(this->data[i*this->sizeColumn + pos]);
    }
}

template <typename NType>
void NMatrix<NType>::getColumn(int pos, NArray<NType>& vec)
{
    if(pos >= this->lenColumn) {throw std::out_of_range("NMatrix<NType>::getColumn() : index is out of range");}

    vec.init(this->lenRow);
    for(int i = 0; i < this->lenRow; i++)
    {
        vec[i] = this->data[i*this->sizeColumn + pos];
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
    int num = sizeRow * sizeColumn;

    if((this->sizeRow != sizeRow)||(this->sizeColumn != sizeColumn))
    {
        NType* p = this->data;
        int oldSizeColumn = this->sizeColumn;
        if(this->lock || this->extrn) {throw std::out_of_range("NMatrix<NType>::resize() : lock size");}

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
void NMatrix<NType>::renew()
{
    if(this->lock || this->extrn) {throw std::out_of_range("NMatrix<NType>::renew() : lock size");}
    this->destruct();
    int num = this->sizeRow * this->sizeColumn;

    if(num == 0)
    {
        this->newmin();
    }
    else
    {
        this->data = new NType[num];
    }
}

template <typename NType>
void NMatrix<NType>::renew(int sizeRow, int sizeColumn)
{
    if(this->lock || this->extrn) {throw std::out_of_range("NMatrix<NType>::renew() : lock size");}
    this->destruct();
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
void NMatrix<NType>::copyFields(NMatrix<NType>& obj)
{
    this->destruct();
    this->data = obj.getData();
    this->extrn = true;

    this->lenRow = obj.getLenRow();
    this->lenColumn = obj.getLenColumn();
    this->sizeRow = obj.getSizeRow();
    this->sizeColumn = obj.getSizeColumn();
    this->lock = obj.getLock();
}

template <typename NType>
void NMatrix<NType>::doMaskRow(bool* mask)
{
    int i, j;

    for(j = 0; j < lenColumn; j++)
    {
        if(mask[j])
        {
            for(i = 0; i < lenRow; i++)
            {
                data[i * sizeColumn + j] = 0;
            }
        }
    }
}

template <typename NType>
void NMatrix<NType>::doMaskColumn(bool* mask)
{
    int i, j;

    for(i = 0; i < lenRow; i++)
    {
        if(mask[i])
        {
            for(j = 0; j < lenColumn; j++)
            {
                data[i * sizeColumn + j] = 0;
            }
        }
    }
}


template <typename NType>
NMatrix<NType>& NMatrix<NType>::sum(NMatrix<NType>& B)
{
    int i, j;
    NType* pB = B.getData();
    int sizeColumnB = B.getSizeColumn();

    for(i = 0; i < lenRow; i++)
    {
        for(j = 0; j < lenColumn; j++)
        {
            data[i*sizeColumn + j] += pB[i*sizeColumnB + j];
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
    int sizeColumnA = A.getSizeColumn();
    int sizeColumnB = B.getSizeColumn();

    this->init(A.getLenRow(), A.getLenColumn());
    for(i = 0; i < lenRow; i++)
    {
        for(j = 0; j < lenColumn; j++)
        {
            data[i*sizeColumn + j] = pA[i*sizeColumnA + j] + pB[i*sizeColumnB + j];
        }
    }
    return (*this);
}

template <typename NType>
NMatrix<NType>& NMatrix<NType>::valsum(const NType& B)
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
NMatrix<NType>& NMatrix<NType>::mul(NMatrix<NType>& B, bool orient)
{
    //return this->mul(*this, B);
    NType* datacpy = nullptr;
    if(data != nullptr)
    {
        int num = lenRow * sizeColumn;
        if(num == 0)
        {
            datacpy = new NType[1];
        }
        else
        {
            datacpy = new NType[num];
            this->memCopy(datacpy, data, sizeColumn, sizeColumn);
        }
    }

    int i, j, k;
    NType value;
    NType* pB = B.getData();
    int sizeColumnA = this->getSizeColumn();
    int sizeColumnB = B.getSizeColumn();
    int lenRowA = this->getLenRow();
    int lenColumnA = this->getLenColumn();

    if(lenRowA == B.getLenColumn() && orient)
    {
        this->init(B.getLenRow(), this->getLenColumn());
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
    else if(lenColumnA == B.getLenRow() && !orient)
    {
        this->init(this->getLenRow(), B.getLenColumn());
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
    else
    {
        throw "NMatrix: size is not mul!";
    }

    if(datacpy != nullptr) {delete[] datacpy;}

    return (*this);
}

template <typename NType>
NMatrix<NType>& NMatrix<NType>::mul(NMatrix<NType>& A, NMatrix<NType>& B, bool orient)
{
    int i, j, k;
    NType value;
    NType* pA = A.getData();
    NType* pB = B.getData();
    int sizeColumnA = A.getSizeColumn();
    int sizeColumnB = B.getSizeColumn();
    int lenRowA = A.getLenRow();
    int lenColumnA = A.getLenColumn();

    if(lenRowA == B.getLenColumn() && orient)
    {
        this->init(B.getLenRow(), A.getLenColumn());
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
    else if(lenColumnA == B.getLenRow() && !orient)
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
    else
    {
        throw "NMatrix: size is not mul!";
    }
    return (*this);
}

template <typename NType>
NMatrix<NType>& NMatrix<NType>::matmul(NMatrix<NType>& B)
{
    int i, j;
    NType* pB = B.getData();
    int sizeColumnB = B.getSizeColumn();

    for(i = 0; i < lenRow; i++)
    {
        for(j = 0; j < lenColumn; j++)
        {
            data[i*sizeColumn + j] *= pB[i*sizeColumnB + j];
        }
    }
    return (*this);
}

template <typename NType>
NMatrix<NType>& NMatrix<NType>::valmul(const NType& B)
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

template <typename NType>
NMatrix<NType>& NMatrix<NType>::valsign()
{
    int i, j;
    for(i = 0; i < lenRow; i++)
    {
        for(j = 0; j < lenColumn; j++)
        {
            data[i*sizeColumn + j] = -data[i*sizeColumn + j];
        }
    }
    return (*this);
}

#endif // NMATRIX_H
