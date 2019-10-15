#ifndef NARRAY_H
#define NARRAY_H

#include "defsmart.h"
#include <mem.h>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <random>
#include <vector>

template <typename NType> class NMatrix;
#include "nmatrix.h"

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
    int length; //Длина массива
    int size; //Размерность массива (выделено памяти)
    int block; //Размер блока, на который выделяется память
    NType* data; //Данные
    bool lock; //Запрет на изменение размерности массива
    bool extrn; //В массиве внешние данные
public:
    void setBlock(int block);
    int getBlock();
    void setLength(int len);
    int getLength();
    int getLengthPlus();
    int getLengthMinus();
    int getLengthPlus(const NType& value);
    int getLengthMinus(const NType& value);
    void setSize(int size);
    int getSize();
    void setData(NType* dt);
    NType* getData();
    void setLock(bool bl);
    bool getLock();
    void setExtrn(bool bl);
    bool getExtrn();
public:
    typedef NType* iterator;
    typedef const NType* const_iterator;
    typedef NType (*funcArray)(NType);
    typedef NType (*funcArrayRef)(NType&);
public:
    NType& at(size_t pos); //Возврат элемента с проверкой
    NType& front(); //Первый элемент
    NType& back(); //Последний элемент
    NArray<NType>::iterator begin() {return this->data;}
    NArray<NType>::iterator end() {return (this->data + this->length);}
    NArray<NType>::const_iterator cbegin() const {return this->data;}
    NArray<NType>::const_iterator cend() const {return (this->data + this->length);}
    NArray<NType>::iterator rbegin() {return (this->data + this->length - 1);}
    NArray<NType>::iterator rend() {return this->data;}
    NArray<NType>::const_iterator crbegin() const {return (this->data + this->length - 1);}
    NArray<NType>::const_iterator crend() const {return this->data;}
    bool empty(); //Массив пустой?
    void shrink_to_fit(); //Очистка неиспользуемой памяти
    NArray<NType>::iterator insert(NArray<NType>::const_iterator pos, const NType& element); //Вставка элемента в позицию
    NArray<NType>::iterator insert(NArray<NType>::const_iterator pos, size_t count, const NType& element); //Вставка элемента в позицию
    NArray<NType>::iterator erase(NArray<NType>::const_iterator pos); //Удаление элемента из позиции
    void swap(NArray<NType>& other); //Обмен элементами между массивами
    void swap(int ind, int jnd); //Поменять элементы местами
    static void srand(); //Инициализация генератора
    static int random_index(int beg, int end);
    int random_index(); //Случайный индекс элемента
    void mix(); //Перемешивание массива
    void mix(int beg, int end);
public:
    void copyValue(NArray<NType>& obj); //Копирование значений
    void destruct(); //Деструктор
    void init(int length); //Инициализация
    void init(int length, const NType& value); //Инициализация значением
    void init_value(const NType& value); //Инициализация значением
    void init_rand(std::default_random_engine& generator, const NType& valMin, const NType& valMax); //Инициализация случайными числами
    void clip(int delta); //Обрезание массива с конца
    void extend(int delta); //Расширение массива с конца
    void extend(int delta, const NType& value); //Расширение массива с конца
    void binary_shold(NType& value); //Применение порога
    void clear(); //Очистка массива
    void add(const NType& element, int pos, int count); //Вставка элемента в позицию
    void add(const NType& element, int pos); //Вставка элемента в позицию
    NType del(int pos); //Удаление элемента из позиции
    void push(const NType& element); //Вставка элемента в конец
    NType pop(); //Извлечение элемента с конца
    void set(const NType& element, int pos); //Установка значения элемента
    NType get(int pos); //Возврат элемента
    int getIndex(const NType& element); //Возврат индекса первого вхождения элемента
    NArray<NType>::iterator find(const NType& element);
    void setSumNormalize(const NType& aver, const NType& element, int pos); //Установка значения с нормализацией
    void resize(int size); //Изменение размерности массива
    void renew(); //Перевыделение памяти (все данные стираются)
    void renew(int size); //Перевыделение памяти (все данные стираются)
    void copyFields(NArray<NType>& obj); //Копирование полей
    void memCopy(NType* src); //Копирование данных массива
    void memCopy(NType* src, int shift, int len);
    void memCopy(NType* dst, NType* src); //Копирование данных массива
    void dataCopy(NType* src); //Поэлементное копирование
    void dataCopy(NType* src, int shift, int len);
    void dataCopy(NType* dst, NType* src); //Поэлементное копирование
    void convertUInt(NArray<unsigned int>& dest); //Конвертация массива uint в элементы массива
    NArray<unsigned int>& toUInt(NArray<unsigned int>& dest); //Конвертация элементов массива в uint
    NType& endElement(); //Последний элемент
    void doMask(bool* mask); //Обнуление элементов по маске
    bool booland(); //Сравнение элементов массива, операция AND
    bool booland(NType valFalse);
    bool boolor(); //Сравнение элементов массива, операция OR
    bool boolor(NType valFalse);
public:
    NType sumElements(); //Сумма элементов
    NType sumElements(int sift);
    NType sumElements(int sift, int num);
    NType maxElements(); //Максимум
    NType maxElements(int sift, int num);
    int maxArg();
    int maxArg(int sift, int num);
    NType minElements(); //Минимум
    NType minElements(int sift, int num);
    int minArg();
    int minArg(int sift, int num);
    NType mean(); //Среднее
    NType std(); //Среднеквадратическое отклонение
    void normalize(); //Нормализация по среднеквадратичному отклонению
    void funcDiv(); //Сумма значений = 1
    void funcExp(); //Вычисление экспоненты
    void funcSoftMax(); //Вычисление softmax
    void funcCalc(funcArray func); //Выполнение произвольной функции
    void funcCalc(funcArrayRef func);
    void funcCalc(NArray<NType>& obj, funcArray func);
    void funcCalc(NArray<NType>& obj, funcArrayRef func);
    NArray<NType>& valsum(const NType& B);
    NArray<NType>& sum(NArray<NType>& B);
    NArray<NType>& sum(NArray<NType>& A, NArray<NType>& B);
    NArray<NType>& valmul(const NType& B);
    NArray<NType>& valsign();
    NArray<NType>& valdiv(const NType& B);
    NArray<NType>& mul(NArray<NType>& B);
    NArray<NType>& mul(NArray<NType>& A, NArray<NType>& B);
    NArray<NType>& div(NArray<NType>& B);
    NArray<NType>& div(NArray<NType>& A, NArray<NType>& B);
    NArray<NType>& prufdiv(NArray<NType>& B, const NType& val);
    NArray<NType>& prufdiv(NArray<NType>& A, NArray<NType>& B, const NType& val);
    NArray<NType>& mul(NMatrix<NType>& B, bool orient);
    NArray<NType>& mul(NArray<NType>& A, NMatrix<NType>& B, bool orient);
    NArray<NType>& mul(NMatrix<NType>& B, NArray<NType>& A, bool orient);
    NArray<NType>& floor();
};


template <typename NType>
NArray<NType>::NArray()
{
    this->data = nullptr;
    this->length = 0;
    this->size = 0;
    this->block = 1;
    this->lock = false;
    this->extrn = false;
}

template <typename NType>
NArray<NType>::NArray(NArray<NType> &obj)
{
    this->length = obj.getLength();
    this->size = obj.getSize();
    this->block = obj.getBlock();
    this->extrn = false;

    this->data = new NType[this->size];

    NType* p = obj.getData();
    if(p != nullptr) {this->dataCopy(p);}
    //this->lock = obj.getLock();
}

template <typename NType>
NArray<NType>& NArray<NType>::operator=(NArray<NType>& obj)
{
    this->length = obj.getLength();
    this->block = obj.getBlock();
    //this->extrn = false;

    if(this->length > this->size)
    {
        this->renew(this->length + this->block);
    }
    NType* p = obj.getData();
    if(p != nullptr) {this->dataCopy(p);}
    //this->lock = obj.getLock();

    return *this;
}

template <typename NType>
NArray<NType>::NArray(int size)
{
    this->data = new NType[size];
    this->length = 0;
    this->size = size;
    this->block = 1;
    this->lock = false;
    this->extrn = false;
}

template <typename NType>
NArray<NType>::NArray(int size, int length, int block)
{
    this->data = new NType[size];
    this->length = length;
    this->size = size;
    this->block = block;
    this->lock = false;
    this->extrn = false;
}

template <typename NType>
NArray<NType>::~NArray()
{
    this->destruct();
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
void NArray<NType>::setBlock(int block)
{
    this->block = block;
}

template <typename NType>
void NArray<NType>::setLength(int len)
{
    this->length = len;
}

template <typename NType>
int NArray<NType>::getLength()
{
    return this->length;
}

template <typename NType>
int NArray<NType>::getLengthPlus()
{
    return this->getLengthPlus(0);
}

template <typename NType>
int NArray<NType>::getLengthMinus()
{
    return this->getLengthMinus(0);
}

template <typename NType>
int NArray<NType>::getLengthPlus(const NType& value)
{
    int len = 0;
    for(NArray<NType>::iterator iter = begin(); iter < end(); iter++)
    {
        if(*iter >= value) {len++;}
    }
    return len;
}

template <typename NType>
int NArray<NType>::getLengthMinus(const NType& value)
{
    int len = 0;
    for(NArray<NType>::iterator iter = begin(); iter < end(); iter++)
    {
        if(*iter < value) {len++;}
    }
    return len;
}

template <typename NType>
void NArray<NType>::setSize(int size)
{
    this->size = size;
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
void NArray<NType>::setData(NType* dt)
{
    this->data = dt;
}

template <typename NType>
NType* NArray<NType>::getData()
{
    return this->data;
}

template <typename NType>
void NArray<NType>::setLock(bool bl)
{
    this->lock = bl;
}

template <typename NType>
bool NArray<NType>::getLock()
{
    return this->lock;
}

template <typename NType>
void NArray<NType>::setExtrn(bool bl)
{
    this->extrn = bl;
}

template <typename NType>
bool NArray<NType>::getExtrn()
{
    return this->extrn;
}


template <typename NType>
NType& NArray<NType>::at(size_t pos)
{
    if(pos >= static_cast<size_t>(this->length)) {throw std::out_of_range("NArray<NType>::at() : index is out of range");}
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
typename NArray<NType>::iterator NArray<NType>::insert(NArray<NType>::const_iterator pos, const NType& element)
{
    if(this->length >= this->size)
    {
        this->resize(this->size + this->block);
    }

    for(NArray<NType>::iterator iter = this->end(); iter != pos; iter--)
    {
        *iter = *(iter-1);
    }
    *pos = element;
    this->length++;
    return pos;
}

template <typename NType>
typename NArray<NType>::iterator NArray<NType>::insert(NArray<NType>::const_iterator pos, size_t count, const NType& element)
{
    if(this->length > this->size + count)
    {
        this->resize(this->size + (int(count/this->block) + 1) * this->block);
    }

    for(NArray<NType>::iterator iter = this->end() + count - 1; iter != pos + count - 1; iter--)
    {
        *iter = *(iter-count);
    }
    for(NArray<NType>::iterator iter = pos; iter != pos + count; iter++)
    {
        *iter = element;
    }
    this->length += count;
    return pos;
}

template <typename NType>
typename NArray<NType>::iterator NArray<NType>::erase(NArray<NType>::const_iterator pos)
{
    this->length--;
    for(NArray<NType>::iterator iter = pos; iter != this->end(); iter++)
    {
        *iter = *(iter+1);
    }
    return pos;
}

template <typename NType>
void NArray<NType>::swap(NArray<NType>& other)
{
    NType* tmpdt = other.getData();
    other.setData(this->data);
    this->data = tmpdt;

    int tmpBlock = other.getBlock();
    int tmpSize = other.getSize();
    int tmpLen = other.getLength();
    bool tmpLock = other.getLock();
    bool tmpExtrn = other.getExtrn();
    other.setBlock(this->block);
    other.setSize(this->size);
    other.setLock(this->lock);
    other.setExtrn(this->extrn);
    other.init(this->length);
    this->block = tmpBlock;
    this->size = tmpSize;
    this->length = tmpLen;
    this->lock = tmpLock;
    this->extrn = tmpExtrn;
}

template <typename NType>
void NArray<NType>::swap(int ind, int jnd)
{
    if(ind != jnd)
    {
        NType tmp = this->data[ind];
        this->data[ind] = this->data[jnd];
        this->data[jnd] = tmp;
    }
}

template <typename NType>
void NArray<NType>::srand()
{
    std::srand(unsigned(std::time(nullptr)));
}

template <typename NType>
int NArray<NType>::random_index(int beg, int end)
{
    return (std::rand() % (end - beg) + beg);
}

template <typename NType>
int NArray<NType>::random_index()
{
    return (std::rand() % this->length);
}

template <typename NType>
void NArray<NType>::mix()
{
    int ind, jnd;
    //NArray<NType>::srand();
    for(int k = 0; k <= this->length / 2; k++)
    {
        ind = this->random_index();
        jnd = this->random_index();
        this->swap(ind, jnd);
    }
}

template <typename NType>
void NArray<NType>::mix(int beg, int end)
{
    int ind, jnd;
    //NArray<NType>::srand();
    for(int k = 0; k <= (end - beg)/2; k++)
    {
        ind = NArray<NType>::random_index(beg, end);
        jnd = NArray<NType>::random_index(beg, end);
        this->swap(ind, jnd);
    }
}


template <typename NType>
void NArray<NType>::copyValue(NArray<NType>& obj)
{
    this->length = obj.getLength();
    this->block = obj.getBlock();
    //this->extrn = false;

    if(this->length > this->size)
    {
        this->renew(this->length + this->block);
    }
    NType* p = obj.getData();
    if(p != nullptr) {this->memCopy(p);}
    //this->lock = obj.getLock();
}

template <typename NType>
void NArray<NType>::destruct()
{
    if(this->data != nullptr && !this->extrn)
    {
        delete[] this->data;
        this->data = nullptr;
    }
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
void NArray<NType>::init(int length, const NType& value)
{
    if(length > this->size)
    {
        //this->resize(length);
        this->renew(length);
    }

    this->length = length;
    this->init_value(value);
}

template <typename NType>
void NArray<NType>::init_value(const NType& value)
{
    for(int i = 0; i < length; i++)
    {
        data[i] = value;
    }
}

template <typename NType>
void NArray<NType>::init_rand(std::default_random_engine& generator, const NType& valMin, const NType& valMax)
{
    std::uniform_real_distribution<NType> distribution(valMin, valMax);
    //NType koef = (valMax - valMin)/(NType)RAND_MAX;
    for(int i = 0; i < length; i++)
    {
        //data[i] = koef * (NType)rand() + valMin;
        data[i] = distribution(generator);
    }
}

template <typename NType>
void NArray<NType>::clip(int delta)
{
    if(delta < this->length) {this->length -= delta;}
    else {this->length = 0;}
}

template <typename NType>
void NArray<NType>::extend(int delta)
{
    this->length += delta;
    if(length > this->size)
    {
        this->resize(length);
    }
}

template <typename NType>
void NArray<NType>::extend(int delta, const NType& value)
{
    this->extend(delta);
    for(int i = length - delta; i < length; i++)
    {
        data[i] = value;
    }
}

template <typename NType>
void NArray<NType>::binary_shold(NType& value)
{
    this->valsum(1-value);
    this->floor();
}

template <typename NType>
void NArray<NType>::clear()
{
    this->length = 0;
}

template <typename NType>
void NArray<NType>::add(const NType& element, int pos, int count)
{
    if(this->length > this->size + count)
    {
        this->resize(this->size + (int(count/this->block) + 1) * this->block);
    }

    for(int i = this->length + count - 1; i > pos + count - 1; i--)
    {
        this->data[i] = this->data[i-count];
    }
    for(int i = pos; i < pos + count; i++)
    {
        this->data[i] = element;
    }
    this->length += count;
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
void NArray<NType>::resize(int size)
{
    size = size > this->length ? size : this->length;
    if(this->size != size)
    {
        if(this->lock || this->extrn) {throw std::out_of_range("NArray<NType>::resize() : lock size");}
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
int NArray<NType>::getIndex(const NType& element)
{
    int ind = 0;
    for(NArray<NType>::iterator iter = begin(); iter < end(); iter++)
    {
        if(*iter == element) {return ind;} ind++;
    }
    return ind;
}

template <typename NType>
typename NArray<NType>::iterator NArray<NType>::find(const NType& element)
{
    for(NArray<NType>::iterator iter = begin(); iter < end(); iter++)
    {
        if(*iter == element) {return iter;}
    }
    return nullptr;
}

template <typename NType>
void NArray<NType>::setSumNormalize(const NType& aver, const NType& element, int pos)
{
    int ind;
    NType deltaPlus = 0, deltaMinus = 0;
    int lenPlus = this->getLengthPlus(aver);
    int lenMinus = this->getLengthMinus(aver);

    if(element >= aver && this->data[pos] >= aver && lenPlus > 1)
    {
        deltaPlus = (this->data[pos] - element) / (lenPlus - 1);
    }
    else if(element < aver && this->data[pos] < aver && lenMinus > 1)
    {
        deltaMinus = (this->data[pos] - element) / (lenMinus - 1);
    }
    else if(element >= aver)
    {
        deltaPlus = (aver - element) / (lenPlus - 1);
        deltaMinus = (this->data[pos] - aver) / (lenMinus - 1);
    }
    else if(element < aver)
    {
        deltaPlus = (this->data[pos] - aver) / (lenPlus - 1);
        deltaMinus = (aver - element) / (lenMinus - 1);
    }

    this->data[pos] = element;
    for(ind = 0; ind < pos; ind++)
    {
        this->data[pos] += (this->data[pos] >= aver ? deltaPlus : deltaMinus);
    }
    for(ind = pos+1; ind < this->length; ind++)
    {
        this->data[pos] += (this->data[pos] >= aver ? deltaPlus : deltaMinus);
    }
}

template <typename NType>
void NArray<NType>::renew()
{
    if(this->lock || this->extrn) {throw std::out_of_range("NArray<NType>::renew() : lock size");}
    this->destruct();
    this->data = new NType[this->size];
}

template <typename NType>
void NArray<NType>::renew(int size)
{
    if(this->lock || this->extrn) {throw std::out_of_range("NArray<NType>::renew() : lock size");}
    this->destruct();
    this->data = new NType[size];
    this->size = size;
}

template <typename NType>
void NArray<NType>::copyFields(NArray<NType>& obj)
{
    this->destruct();
    this->data = obj.getData();
    this->extrn = true;

    this->length = obj.getLength();
    this->size = obj.getSize();
    this->block = obj.getBlock();
    this->lock = obj.getLock();
}

template <typename NType>
void NArray<NType>::memCopy(NType* src)
{
    memcpy(this->data, src, this->length * sizeof(NType));
}

template <typename NType>
void NArray<NType>::memCopy(NType* src, int shift, int len)
{
    memcpy(this->data + shift, src, len * sizeof(NType));
}

template <typename NType>
void NArray<NType>::memCopy(NType* dst, NType* src)
{
    memcpy(dst, src, this->length * sizeof(NType));
}

template <typename NType>
void NArray<NType>::dataCopy(NType* src)
{
    for(int i = 0; i < length; i++)
    {
        data[i] = src[i];
    }
}

template <typename NType>
void NArray<NType>::dataCopy(NType* src, int shift, int len)
{
    for(int i = 0; i < len; i++)
    {
        data[i + shift] = src[i];
    }
}

template <typename NType>
void NArray<NType>::dataCopy(NType* dst, NType* src)
{
    for(int i = 0; i < length; i++)
    {
        dst[i] = src[i];
    }
}

template <typename NType>
void NArray<NType>::convertUInt(NArray<unsigned int>& dest)
{
    this->clear();
    for(int i = 0; i < dest.getLength(); i++)
    {
        this->push(static_cast<NType>(dest[i]));
    }
}

template <typename NType>
NArray<unsigned int>& NArray<NType>::toUInt(NArray<unsigned int>& dest)
{
    dest.clear();
    for(int i = 0; i < this->length; i++)
    {
        dest.push(static_cast<unsigned int>(this->data[i]));
    }
    return dest;
}


template <typename NType>
NType& NArray<NType>::endElement()
{
    return *(this->data + this->length - 1);
}

template <typename NType>
void NArray<NType>::doMask(bool* mask)
{
    for(int i = 0; i < length; i++)
    {
        if(mask[i]) {data[i] = 0;}
    }
}

template <typename NType>
bool NArray<NType>::booland()
{
    bool bl = true;
    int ind = 0;
    while(bl && ind < length) {bl = data[ind]; ind++;}
    return bl;
}

template <typename NType>
bool NArray<NType>::booland(NType valFalse)
{
    bool bl = true;
    int ind = 0;
    while(bl && ind < length) {bl = (data[ind] != valFalse); ind++;}
    return bl;
}

template <typename NType>
bool NArray<NType>::boolor()
{
    bool bl = false;
    int ind = 0;
    while(!bl && ind < length) {bl = data[ind]; ind++;}
    return bl;
}

template <typename NType>
bool NArray<NType>::boolor(NType valFalse)
{
    bool bl = false;
    int ind = 0;
    while(!bl && ind < length) {bl = (data[ind] != valFalse); ind++;}
    return bl;
}

template <typename NType>
NType NArray<NType>::sumElements()
{
    NType total = 0;
    for(int i = 0; i < length; i++)
    {
        total += data[i];
    }
    return total;
}

template <typename NType>
NType NArray<NType>::sumElements(int shift)
{
    NType total = 0;
    for(int i = shift; i < length; i++)
    {
        total += data[i];
    }
    return total;
}

template <typename NType>
NType NArray<NType>::sumElements(int shift, int num)
{
    NType total = 0;
    for(int i = 0; i < num; i++)
    {
        total += data[i+shift];
    }
    return total;
}

template <typename NType>
NType NArray<NType>::maxElements()
{
    NType total = data[0];
    for(int i = 1; i < length; i++)
    {
        if(data[i] > total) {total = data[i];}
    }
    return total;
}

template <typename NType>
NType NArray<NType>::maxElements(int sift, int num)
{
    NType total = data[sift];
    for(int i = 1; i < num; i++)
    {
        if(data[i+sift] > total) {total = data[i+sift];}
    }
    return total;
}

template <typename NType>
int NArray<NType>::maxArg()
{
    int index = 0;
    for(int i = 1; i < length; i++)
    {
        if(data[i] > data[index]) {index = i;}
    }
    return index;
}

template <typename NType>
int NArray<NType>::maxArg(int sift, int num)
{
    int index = sift;
    for(int i = 1; i < num; i++)
    {
        if(data[i+sift] > data[index]) {index = i+sift;}
    }
    return index;
}

template <typename NType>
NType NArray<NType>::minElements()
{
    NType total = data[0];
    for(int i = 1; i < length; i++)
    {
        if(data[i] < total) {total = data[i];}
    }
    return total;
}

template <typename NType>
NType NArray<NType>::minElements(int sift, int num)
{
    NType total = data[sift];
    for(int i = 1; i < num; i++)
    {
        if(data[i+sift] < total) {total = data[i+sift];}
    }
    return total;
}

template <typename NType>
int NArray<NType>::minArg()
{
    int index = 0;
    for(int i = 1; i < length; i++)
    {
        if(data[i] < data[index]) {index = i;}
    }
    return index;
}

template <typename NType>
int NArray<NType>::minArg(int sift, int num)
{
    int index = sift;
    for(int i = 1; i < num; i++)
    {
        if(data[i+sift] < data[index]) {index = i+sift;}
    }
    return index;
}

template <typename NType>
NType NArray<NType>::mean()
{
    return this->sumElements() / this->length;
}

template <typename NType>
NType NArray<NType>::std()
{
    NType result = 0;
    NType aver = this->mean();
    for(int ind = 0; ind < length; ind++)
    {
        result += NFUNC_SQR(data[ind] - aver);
    }
    return NFUNC_SQRT(result / this->length);
}

template <typename NType>
void NArray<NType>::normalize()
{
    int ind;
    NType std_result = 0;
    NType aver = this->mean();
    for(ind = 0; ind < length; ind++)
    {
        std_result += NFUNC_SQR(data[ind] - aver);
    }
    std_result = NFUNC_SQRT(std_result / this->length);

    for(ind = 0; ind < length; ind++)
    {
        data[ind] = (data[ind] - aver) / std_result;
    }
}

template <typename NType>
void NArray<NType>::funcDiv()
{
    this->valdiv(this->sumElements());
}

template <typename NType>
void NArray<NType>::funcExp()
{
    for(int ind = 0; ind < length; ind++)
    {
        data[ind] = NFUNC_EXP(data[ind]);
    }
}

template <typename NType>
void NArray<NType>::funcSoftMax()
{
    this->valsum(-this->maxElements());
    this->funcExp();
    this->funcDiv();
}

template <typename NType>
void NArray<NType>::funcCalc(funcArray func)
{
    for(int ind = 0; ind < length; ind++)
    {
        data[ind] = func(data[ind]);
    }
}

template <typename NType>
void NArray<NType>::funcCalc(funcArrayRef func)
{
    for(int ind = 0; ind < length; ind++)
    {
        data[ind] = func(data[ind]);
    }
}

template <typename NType>
void NArray<NType>::funcCalc(NArray<NType>& obj, funcArray func)
{
    this->length = obj.getLength();
    this->block = obj.getBlock();
    //this->extrn = false;

    if(this->length > this->size)
    {
        this->renew(this->length + this->block);
    }

    NType* pObj = obj.getData();
    if(pObj != nullptr)
    {
        for(int ind = 0; ind < length; ind++)
        {
            data[ind] = func(pObj[ind]);
        }
    }
    //this->lock = obj.getLock();
}

template <typename NType>
void NArray<NType>::funcCalc(NArray<NType>& obj, funcArrayRef func)
{
    this->length = obj.getLength();
    this->block = obj.getBlock();
    //this->extrn = false;

    if(this->length > this->size)
    {
        this->renew(this->length + this->block);
    }

    NType* pObj = obj.getData();
    if(pObj != nullptr)
    {
        for(int ind = 0; ind < length; ind++)
        {
            data[ind] = func(pObj[ind]);
        }
    }
    //this->lock = obj.getLock();
}

template <typename NType>
NArray<NType>& NArray<NType>::valsum(const NType& B)
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
NArray<NType>& NArray<NType>::valmul(const NType& B)
{
    for(int i = 0; i < length; i++)
    {
        data[i] *= B;
    }
    return (*this);
}

template <typename NType>
NArray<NType>& NArray<NType>::valsign()
{
    for(int i = 0; i < length; i++)
    {
        data[i] = -data[i];
    }
    return (*this);
}

template <typename NType>
NArray<NType>& NArray<NType>::valdiv(const NType& B)
{
    for(int i = 0; i < length; i++)
    {
        data[i] /= B;
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
        datacpy = new NType[length];
        this->memCopy(datacpy, data);
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
    else
    {
        throw "NArray: size is not mul!";
    }

    if(datacpy != nullptr) {delete[] datacpy;}

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
NArray<NType>& NArray<NType>::div(NArray<NType>& B)
{
    NType* pB = B.getData();
    for(int i = 0; i < length; i++)
    {
        data[i] /= pB[i];
    }
    return (*this);
}

template <typename NType>
NArray<NType>& NArray<NType>::div(NArray<NType>& A, NArray<NType>& B)
{
    NType* pA = A.getData();
    NType* pB = B.getData();
    for(int i = 0; i < length; i++)
    {
        data[i] = pA[i] / pB[i];
    }
    return (*this);
}

template <typename NType>
NArray<NType>& NArray<NType>::prufdiv(NArray<NType>& B, const NType& val)
{
    NType* pB = B.getData();
    for(int i = 0; i < length; i++)
    {
        data[i] = (pB[i] == 0 ? val : data[i] / pB[i]);
    }
    return (*this);
}

template <typename NType>
NArray<NType>& NArray<NType>::prufdiv(NArray<NType>& A, NArray<NType>& B, const NType& val)
{
    NType* pA = A.getData();
    NType* pB = B.getData();
    for(int i = 0; i < length; i++)
    {
        data[i] = (pB[i] == 0 ? val : pA[i] / pB[i]);
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
    else
    {
        throw "NArray: size is not mul!";
    }
    return (*this);
}

template <typename NType>
NArray<NType>& NArray<NType>::mul(NMatrix<NType>& B, NArray<NType>& A, bool orient)
{
    return this->mul(A, B, orient);
}

template <typename NType>
NArray<NType>& NArray<NType>::floor()
{
    for(int i = 0; i < length; i++)
    {
        data[i] = std::floor(data[i]);
    }
}

#endif // NARRAY_H
