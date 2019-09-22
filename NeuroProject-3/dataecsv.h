#ifndef DATAECSV_H
#define DATAECSV_H

#include "structecsv.h"
#include <iostream>
#include <fstream>

class DataECSV
{
public:
    vector<StructECSV*> modules; //Блоки файла
    vector<size_t> indexModule; //Индексы блоков
    vector<long> indexString; //Индексы строк (указатели места)
    vector<string>* fieldsNotSave; //Указатель на поля
protected:
    size_t shift; //Указатель на смещение (текущий индекс модуля)
    long indexSize; //Размер шапки файла. Определяется после чтения или записи шапки.
    string delim_data; //Разделитель данных
    bool array_br; //Выводить массив с новой строки?

public:
    DataECSV();

public:
    void setShift(size_t ind);
    size_t getShift();
    size_t getIndexSize();
    void setDelimData(string& delim);
    string getDelimData();
    void setArrayBR(bool bl);
    void beginShift(); //Переместить указатель в начало
    void endShift(); //Переместить указатель за последний элемент

public:
    bool isSpec(string& str); //Это специализированная строка?
    string& getSpec(string& str); //Возвращает строку без управляющего символа
    bool getGroup(string& str); //Возвращает строку без []
    bool isDataPath(StructECSV* block); //Данные могут расположиться в файле вместе с путём (одной строкой)?
    bool isNextBlock(size_t ind, string& parent); //Пока есть следующий блок
    void clear(); //Очистить всё
    void clearData(); //Очистить данные
    void clearIndex(); //Очистить индексы
    StructECSV* addDataMatrix(StructECSV* block, string& str); //Добавить строку данных в матрицу блока
    bool isOneMatrix(); //Блоки содержат одну матрицу? (Проверка на CSV файл)
    void seek_back(fstream& sm, long skback); //Перемещение указателя по отношению к началу файла
    void shiftIndexString(fstream& index_file); //Добавление к индексу размера индексного файла

    bool create(string name); //Создание пустого файла
    bool read(string name); //Чтение данных из указанного файла
    bool write(string name); //Запись данных в указанный файл
    void readBody(fstream& in_file, long seekstop);
    void writeBody(fstream& out_file);
    bool readHead(string name); //Чтение шапки файла
    void readHead(fstream& in_file);
    bool writeHead(string name); //Запись шапки файла
    void writeHead(fstream& out_file);
    bool readObj(string name, size_t num); //Чтение объекта
    void readObj(fstream& in_file, size_t num);
    bool appendObj(string name); //Запись в конец файла
    void appendObj(fstream& out_file);
    void seekr(fstream& in_file, size_t ind); //Позиционирование на заданный объект
    void seekw(fstream& out_file, bool seq); //Запись позиции текущего объекта
    bool unionHeadBody(string index_name, string body_name); //Объединение индексного файла и файла с данными
    void unionHeadBody(fstream& index_file, fstream& body_file);

protected:
    bool getline(fstream& in_file, string& str); //Чтение строки из файла

public:
    StructECSV* addElement(string& parent, const string& field, string& value, string& type); //Добавление нового блока-элемента
    StructECSV* addElement(string& parent, const string& field, vector<string>& value, string& type);
    StructECSV* addElement(string& parent, const string& field, NArray<string>& value, string& type);
    StructECSV* addElement(string& parent, const string& field, NMatrix<string>& value, string& type);

    StructECSV* addElement(string& parent, const string& field, string& value, const char* type);
    StructECSV* addElement(string& parent, const string& field, vector<string>& value, const char* type);
    StructECSV* addElement(string& parent, const string& field, NArray<string>& value, const char* type);
    StructECSV* addElement(string& parent, const string& field, NMatrix<string>& value, const char* type);

    StructECSV* addString(string& parent, const string& field, string& value); //Добавление нового блока-строки
    StructECSV* addString(string& parent, const string& field, vector<string>& value);
    StructECSV* addString(string& parent, const string& field, NArray<string>& value);
    StructECSV* addString(string& parent, const string& field, NMatrix<string>& value);

    StructECSV* addGroup(string& parent, string& type);
    StructECSV* addGroup(string& parent, const char* type);
};

#endif // DATAECSV_H
