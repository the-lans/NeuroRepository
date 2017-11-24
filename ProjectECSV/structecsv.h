#ifndef STRUCTECSV_H
#define STRUCTECSV_H

#include "funcecsv.h"

using namespace std;

struct StructECSV
{
    vector<string> path; //Путь к данным
    bool group; //Это группа
    vector<string> type; //Тип данных
    NMatrix<string> mtrx; //Основная матрица данных
    vector<TypeDataECSV> typeECSV; //Тип файла ECSV

    StructECSV();

    TypeDataECSV getTypeECSV(int ind); //Вернуть используемый тип ECSV
    string& getData(string& data, const char* delim); //Вернуть матрицу данных в виде строки
    string& getData(string& data, string& delim);
    string& getType(string& str, const char* delim); //Вернуть типы данных в виде строки
    string& getType(string& str, string& delim);
    string getPath(); //Вернуть путь в виде строки
    bool cmpPath(string& str, bool acc); //Сравнить путь со строкой
    bool cmpPath(vector<string>& subpath, bool acc); //Сравнить путь с массивом строк
    vector<string>& splitPath(string& str); //Разобрать путь на подстроки по разделителю
    bool isFieldObj(string& parent); //Это поле принадлежит родителю?
    void addDataMatrix(string& str, const char* delim); //Добавить строку данных
    void addDataMatrix(string& str, string& delim);
    void clear(); //Очистка текущего блока
    string& getMatrixValue(string& value); //Вернуть одно значение из основной матрицы
    string getMatrixValue();
    vector<string>& getMatrixVector(vector<string>& value); //Вернуть вектор из основной матрицы
    NArray<string>& getMatrixVector(NArray<string>& value); //Вернуть вектор из основной матрицы
    bool isField(string& parent, const string& field); //Поле совпадает с блоком?
    bool getFieldValue(string& parent, const string& field, NMatrix<string>& value); //Возврат матрицы из блока
    bool getFieldValue(string& parent, const string& field, vector<string>& value); //Возврат вектора из блока
    bool getFieldValue(string& parent, const string& field, NArray<string>& value); //Возврат вектора из блока
    bool getFieldValue(string& parent, const string& field, string& value); //Возврат одного значения из блока
};

#endif // STRUCTECSV_H
