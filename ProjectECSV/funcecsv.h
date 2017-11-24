#ifndef FUNCECSV_H
#define FUNCECSV_H

//#include "narray.h"
#include "nmatrix.h"
#include <string>
#include <vector>
#include <typeinfo>
#include <ctype.h>
#include <cstdlib>

using namespace std;

enum class TypeDataECSV {Element, String};

bool* newbool(int n, bool val); //Новый массив bool
void boolinit(bool* mas, int n, bool val); //Инициализация массива bool
bool booland(bool* mas, int n); //Сравнение элементов массива, операция AND
bool boolor(bool* mas, int n); //Сравнение элементов массива, операция OR

bool cmpstrslash(const string& str, size_t ind1, size_t ind2); //Сравнение с символом экранирования
void split(vector<string>& dest, const string& str, const char* delim); //Разделение строки на подстроки по разделителю
void split_no_slash(vector<string>& dest, const string& str, const char* delim); //Разделение строки на подстроки по разделителю с учётом символа экранирования
void concat(string& str, vector<string>& dest, const char* delim); //Сбор строки из подстрок
void concat(string& str, string* dest, int len, const char* delim); //Сбор строки из подстрок
string& trimstr(string& s); //Удаление концевых пробелов
TypeDataECSV str_to_element(string& dest, const string& str); //Преобразование строки в элемент
vector<TypeDataECSV> str_to_element(vector<string>& vecstr); //Преобразование строки в элемент
string& str_to_lower(string& str); //Преобразование символов в нижний регистр
size_t delcomment(string& dest); //Удаление комментариев в строке
void shield(string& dest, const string& str); //Экранирование
void shield(vector<string>& vecstr); //Экранирование
void freeshield(string& dest, const string& str); //Преобразование экранированных символов
void freeshield(vector<string>& vecstr); //Преобразование экранированных символов

void to_value(double& value, string& str_val); //Преобразование строки в значение
void to_value(float& value, string& str_val);
void to_value(signed int& value, string& str_val);
void to_value(unsigned int& value, string& str_val);
void to_value(long& value, string& str_val);
void to_value(bool& value, string str_val);

string to_vstring(bool& value); //Преобразование значения в строку

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
void to_array_string(vector<string>& dest, vector<NType>& vec)
{
    dest.clear();
    for(size_t ind = 0; ind < vec.size(); ind++)
    {
        dest.push_back(to_string(vec[ind]));
    }
}

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
void to_array_vstring(vector<string>& dest, vector<NType>& vec)
{
    dest.clear();
    for(size_t ind = 0; ind < vec.size(); ind++)
    {
        dest.push_back(to_vstring(vec[ind]));
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
void to_array_value(vector<NType>& dest, vector<string>& str_vec)
{
    NType val;
    dest.clear();
    for(size_t ind = 0; ind < str_vec.size(); ind++)
    {
        to_value(val, str_vec[ind]);
        dest.push_back(val);
    }
}

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

template <typename NType>
void toUInt(vector<unsigned int>& enm_arr, vector<NType>& vec)
{
    enm_arr.clear();
    for(size_t i = 0; i < vec.size(); i++)
    {
        enm_arr.push_back((unsigned int)vec[i]);
    }
}

template <typename NType>
void convertUInt(vector<NType>& vec, vector<unsigned int>& enm_arr)
{
    vec.clear();
    for(size_t i = 0; i < enm_arr.size(); i++)
    {
        vec.push_back((NType)enm_arr[i]);
    }
}

#endif // FUNCECSV_H
