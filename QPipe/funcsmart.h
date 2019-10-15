#ifndef FUNCSMART_H
#define FUNCSMART_H

#include "defsmart.h"
#include <mem.h>
#include <string>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <ctype.h>
#include <cstdlib>

using namespace std;

bool* newbool(int n, bool val); //Новый массив bool
void boolinit(bool* mas, int n, bool val); //Инициализация массива bool
bool booland(bool* mas, int n); //Сравнение элементов массива, операция AND
bool boolor(bool* mas, int n); //Сравнение элементов массива, операция OR

string getpath(string parent, string& field, const char sym = '.'); //Возвращает цельный путь
string getpath(string parent, const string& field, const char sym = '.');
bool resolvedfield(vector<string>* fieldsNotSave, string& parent, string& field); //Разрешённое поле
bool resolvedfield(vector<string>* fieldsNotSave, string& parent, const string& field);
bool cmppath(const string& str, const string& strsub, bool acc, const char* sym = "."); //Подстрока входит в строку
bool cmppath(vector<string>& path, vector<string>& subpath, bool acc);
bool cmpstrslash(const string& str, size_t ind1, size_t ind2); //Сравнение с символом экранирования
void split(vector<string>& dest, const string& str, const char* delim); //Разделение строки на подстроки по разделителю
void split(vector<string>& dest, string str, vector<string>& delim);
void splitno(vector<string>& dest, const string& str, const char* delim);
void splitno(vector<string>& dest, string str, vector<string>& delim);
void split_no_slash(vector<string>& dest, const string& str, const char* delim); //Разделение строки на подстроки по разделителю с учётом символа экранирования
void concat(string& str, vector<string>& dest, const char* delim); //Сбор строки из подстрок
void concat(string& str, string* dest, int len, const char* delim); //Сбор строки из подстрок
void replacestr(string& str, const string& src, const string& dst); //Замена подстроки
string& trimstr(string& s); //Удаление концевых пробелов
int trimBOM_utf8(string& s);
string& str_to_lower(string& str); //Преобразование символов в нижний регистр
size_t delcomment(string& dest); //Удаление комментариев в строке
void shield(string& dest, const string& str); //Экранирование
void shield(vector<string>& vecstr); //Экранирование
void freeshield(string& dest, const string& str); //Преобразование экранированных символов
void freeshield(vector<string>& vecstr); //Преобразование экранированных символов
bool isnumber(const string& str); //Это число?

void to_value(double& value, string& str_val); //Преобразование строки в значение
void to_value(float& value, string& str_val);
void to_value(signed int& value, string& str_val);
void to_value(unsigned int& value, string& str_val);
void to_value(long& value, string& str_val);
void to_value(bool& value, string str_val);
void to_value(string& value, string& str_val);
string to_string(string& val);

string to_vstring(bool& value); //Преобразование значения в строку


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
void to_array_vstring(vector<string>& dest, vector<NType>& vec)
{
    dest.clear();
    for(size_t ind = 0; ind < vec.size(); ind++)
    {
        dest.push_back(to_vstring(vec[ind]));
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

#endif // FUNCSMART_H
