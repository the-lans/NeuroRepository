#ifndef DATAECSV_H
#define DATAECSV_H

#include "structecsv.h"
#include <iostream>
#include <fstream>

class DataECSV
{
public:
    vector<StructECSV*> modules; //Блоки файла
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
    bool isDataPath(StructECSV* block); //Данные могут расположиться в файле вместе с путём (одной строкой)?
    vector<string>& splitPath(vector<string>& dest, string& str); //Разобрать путь на подстроки по разделителю
    void clear(); //Очистить всё
    StructECSV* addDataMatrix(StructECSV* block, string& str); //Добавить строку данных в матрицу блока
    bool isOneMatrix(); //Блоки содержат одну матрицу? (Проверка на CSV файл)
    bool read(string name); //Чтение данных из указанного файла
    bool write(string name); //Запись данных в указанный файл

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
};

#endif // DATAECSV_H
