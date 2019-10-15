#ifndef FUNCECSV_H
#define FUNCECSV_H

//#include "narray.h"
#include "nmatrix.h"
#include "funcsmart.h"
#include <iostream>
#include <fstream>
#include <sstream>

enum class TypeDataECSV {Element, String};

void readFile(string& str, const string& fileName); //Чтение файла в строку
TypeDataECSV str_to_element(string& dest, const string& str); //Преобразование строки в элемент
vector<TypeDataECSV> str_to_element(vector<string>& vecstr); //Преобразование строки в элемент
void split(NArray<string>& dest, const string& str, const char* delim); //Разделение строки на подстроки по разделителю
void split(NArray<string>& dest, string str, NArray<string>& delim);
void splitno(NArray<string>& dest, const string& str, const char* delim);
void splitno(NArray<string>& dest, string str, NArray<string>& delim);
void concat(string& str, NArray<string>& dest, const char* delim); //Сбор строки из подстрок


//Преобразование матрицы значений в матрицу строк
template <typename NType>
void to_matrix_string(NMatrix<string>& dest, NMatrix<NType>& mtrx)
{
    int ind, jnd;
    int lenRow = mtrx.getLenRow();
    int lenColumn = mtrx.getLenColumn();

    dest.init(lenRow, lenColumn);
    for(ind = 0; ind < lenRow; ind++)
    {
        for(jnd = 0; jnd < lenColumn; jnd++)
        {
            dest[ind][jnd] = to_string(mtrx[ind][jnd]);
        }
    }
}

template <typename NType>
void to_matrix_string(NMatrix<string>& dest, vector<NType>& vec)
{
    int jnd;
    int lenColumn = vec.size();

    dest.init(1, lenColumn);
    for(jnd = 0; jnd < lenColumn; jnd++)
    {
        dest[0][jnd] = to_string(vec[jnd]);
    }
}

//Преобразование массива значений в массив строк
template <typename NType>
void to_array_string(vector<string>& dest, NArray<NType>& vec)
{
    dest.clear();
    for(int ind = 0; ind < vec.getLength(); ind++)
    {
        dest.push_back(to_string(vec[ind]));
    }
}

template <typename NType>
void to_array_vstring(vector<string>& dest, NArray<NType>& vec)
{
    dest.clear();
    for(int ind = 0; ind < vec.getLength(); ind++)
    {
        dest.push_back(to_vstring(vec[ind]));
    }
}

//Преобразование матрицы строк в матрицу значений
template <typename NType>
void to_matrix_value(NMatrix<NType>& dest, NMatrix<string>& str_mtrx)
{
    int ind, jnd;
    int lenRow = str_mtrx.getLenRow();
    int lenColumn = str_mtrx.getLenColumn();

    dest.init(lenRow, lenColumn);
    for(ind = 0; ind < lenRow; ind++)
    {
        for(jnd = 0; jnd < lenColumn; jnd++)
        {
            to_value(dest[ind][jnd], str_mtrx[ind][jnd]);
        }
    }
}

template <typename NType>
void to_matrix_value(vector<NType>& dest, NMatrix<string>& str_mtrx)
{
    int jnd;
    NType val;
    int lenColumn = str_mtrx.getLenColumn();

    dest.clear();
    for(jnd = 0; jnd < lenColumn; jnd++)
    {
        to_value(val, str_mtrx[0][jnd]);
        dest.push_back(val);
    }
}

//Преобразование массива строк в массив значений
template <typename NType>
void to_array_value(NArray<NType>& dest, vector<string>& str_vec)
{
    NType val;
    dest.clear();
    for(size_t ind = 0; ind < str_vec.size(); ind++)
    {
        to_value(val, str_vec[ind]);
        dest.push(val);
    }
}

#endif // FUNCECSV_H
