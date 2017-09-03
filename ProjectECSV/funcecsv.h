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

void to_matrix_string(NMatrix<string>& dest, NMatrix<double>& mtrx); //Преобразование матрицы значений в матрицу строк
void to_matrix_string(NMatrix<string>& dest, NMatrix<float>& mtrx);
void to_matrix_string(NMatrix<string>& dest, NMatrix<unsigned int>& mtrx);
void to_matrix_string(NMatrix<string>& dest, NMatrix<signed int>& mtrx);
void to_matrix_string(NMatrix<string>& dest, vector<double>& vec);
void to_matrix_string(NMatrix<string>& dest, vector<float>& vec);
void to_matrix_string(NMatrix<string>& dest, vector<unsigned int>& vec);
void to_matrix_string(NMatrix<string>& dest, vector<signed int>& vec);

void to_array_string(vector<string>& dest, vector<double>& vec); //Преобразование массива значений в массив строк
void to_array_string(vector<string>& dest, vector<float>& vec);
void to_array_string(vector<string>& dest, vector<unsigned int>& vec);
void to_array_string(vector<string>& dest, vector<signed int>& vec);
void to_array_string(vector<string>& dest, NArray<double>& vec);
void to_array_string(vector<string>& dest, NArray<float>& vec);
void to_array_string(vector<string>& dest, NArray<unsigned int>& vec);
void to_array_string(vector<string>& dest, NArray<signed int>& vec);

void to_matrix_value(NMatrix<double>& dest, NMatrix<string>& str_mtrx); //Преобразование матрицы строк в матрицу значений
void to_matrix_value(NMatrix<float>& dest, NMatrix<string>& str_mtrx);
void to_matrix_value(NMatrix<unsigned int>& dest, NMatrix<string>& str_mtrx);
void to_matrix_value(NMatrix<signed int>& dest, NMatrix<string>& str_mtrx);
void to_matrix_value(vector<double>& dest, NMatrix<string>& str_mtrx);
void to_matrix_value(vector<float>& dest, NMatrix<string>& str_mtrx);
void to_matrix_value(vector<unsigned int>& dest, NMatrix<string>& str_mtrx);
void to_matrix_value(vector<signed int>& dest, NMatrix<string>& str_mtrx);

void to_array_value(vector<double>& dest, vector<string>& str_vec); //Преобразование массива строк в массив значений
void to_array_value(vector<float>& dest, vector<string>& str_vec);
void to_array_value(vector<unsigned int>& dest, vector<string>& str_vec);
void to_array_value(vector<signed int>& dest, vector<string>& str_vec);
void to_array_value(NArray<double>& dest, vector<string>& str_vec);
void to_array_value(NArray<float>& dest, vector<string>& str_vec);
void to_array_value(NArray<unsigned int>& dest, vector<string>& str_vec);
void to_array_value(NArray<signed int>& dest, vector<string>& str_vec);

void to_value(double& value, string& str_val); //Преобразование строки в значение
void to_value(float& value, string& str_val);
void to_value(signed int& value, string& str_val);
void to_value(unsigned int& value, string& str_val);
void to_value(long& value, string& str_val);


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
