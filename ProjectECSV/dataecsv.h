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
protected:
    size_t shift; //Указатель на смещение (текущий индекс модуля)
    string delim_data; //Разделитель данных
    bool array_br; //Выводить массив с новой строки?

public:
    DataECSV();

public:
    void setShift(size_t ind);
    size_t getShift();
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
    StructECSV* addDataMatrix(StructECSV* block, string& str); //Добавить строку данных в матрицу блока
    bool isOneMatrix(); //Блоки содержат одну матрицу? (Проверка на CSV файл)
    void seek_back(ifstream& sm, long skback); //Перемещение указателя по отношению к началу файла

    bool read(string name); //Чтение данных из указанного файла
    bool write(string name); //Запись данных в указанный файл
    void readBody(ifstream& in_file, long seekstop);
    void writeBody(ofstream& out_file);
    bool readHead(string name); //Чтение шапки файла
    void readHead(ifstream& in_file);
    bool writeHead(string name); //Запись шапки файла
    void writeHead(ofstream& out_file);
    bool readObj(string name, size_t num); //Чтение объекта
    void readObj(ifstream& in_file, size_t num);
    bool appendObj(string name); //Запись в конец файла
    void appendObj(ofstream& out_file);
    void seekr(ifstream& in_file, size_t ind); //Позиционирование на заданный объект
    void seekw(ofstream& out_file, bool seq); //Запись позиции текущего объекта
    bool unionHeadBody(string index_name, string body_name); //Объединение индексного файла и файла с данными
    void unionHeadBody(ofstream& index_file, ifstream& body_file);

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
