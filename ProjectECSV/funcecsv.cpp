#include "funcecsv.h"

//Новый массив bool
bool* newbool(int n, bool val)
{
    bool* mas = new bool[n];
    boolinit(mas, n, val);
    return mas;
}

//Инициализация массива bool
void boolinit(bool* mas, int n, bool val)
{
    for(int i = 0; i < n; i++) {mas[i] = val;}
}

//Сравнение элементов массива, операция AND
bool booland(bool* mas, int n)
{
    bool bl = true;
    int ind = 0;
    while(bl && ind < n) {bl = mas[ind]; ind++;}
    return bl;
}

//Сравнение элементов массива, операция OR
bool boolor(bool* mas, int n)
{
    bool bl = (n > 0 ? false : true);
    int ind = 0;
    while(!bl && ind < n) {bl = mas[ind]; ind++;}
    return bl;
}

//Сравнение с символом экранирования
bool cmpstrslash(const string& str, size_t ind1, size_t ind2)
{
    if(ind2 > 0 && str[ind2-1] == '\\')
    {
        size_t ind = ind1;
        while(ind < ind2) {if(str[ind] == '\\') {ind++;} ind++;}
        if(ind > ind2) {return true;}
    }
    return false;
}

//Разделение строки на подстроки по разделителю
void split(vector<string>& dest, const string& str, const char* delim)
{
    size_t cout_delim = strlen(delim);
    size_t ind1 = 0, ind2 = 0;
    string substr;

    while(ind1 < str.length() && ind2 != std::string::npos)
    {
        ind2 = str.find(delim, ind1, cout_delim);
        if(ind2 == std::string::npos) {ind2 = str.length();}
        substr = (ind1 == ind2 ? "" : str.substr(ind1, ind2 - ind1));
        dest.push_back(substr);
        ind1 = ind2 + cout_delim;
    }

    if(ind2 < str.length()) {dest.push_back("");}
}

//Разделение строки на подстроки по разделителю с учётом символа экранирования
void split_no_slash(vector<string>& dest, const string& str, const char* delim)
{
    size_t cout_delim = strlen(delim);
    size_t ind1 = 0, ind2 = 0, poisk = 0;
    string substr;

    while(ind1 < str.length() && ind2 != std::string::npos)
    {
        ind2 = str.find(delim, poisk, cout_delim);
        if(ind2 != std::string::npos && cmpstrslash(str, ind1, ind2))
        {
            poisk = ind2 + 1;
        }
        else
        {
            if(ind2 == std::string::npos) {ind2 = str.length();}
            substr = (ind1 == ind2 ? "" : str.substr(ind1, ind2 - ind1));
            dest.push_back(substr);
            poisk = ind2 + cout_delim;
            ind1 = poisk;
        }
    }

    if(ind2 < str.length()) {dest.push_back("");}
}

//Сбор строки из подстрок
void concat(string& str, vector<string>& dest, const char* delim)
{
    str = "";
    for(vector<string>::iterator iter = dest.begin(); iter != dest.end(); iter++)
    {
        str.append(*iter);
        str.append(delim);
    }
    size_t len_delim = strlen(delim);
    str.erase(str.length()-len_delim, len_delim);
}

//Сбор строки из подстрок
void concat(string& str, string* dest, int len, const char* delim)
{
    str = "";
    for(int i = 0; i < len; i++)
    {
        str.append(dest[i]);
        str.append(delim);
    }
    size_t len_delim = strlen(delim);
    str.erase(str.length()-len_delim, len_delim);
}

//Удаление концевых пробелов
string& trimstr(string& s)
{
    const string SPACES(" \r\t\n");
    size_t head = s.find_first_not_of(SPACES);
    if(head == string::npos) {s = "";}
    else if(head > 0) {s.erase(0, head);}
    size_t tail = s.find_last_not_of(SPACES);
    if(tail != s.size() - 1) {s.erase(tail + 1);}
    return s;
}

//Преобразование строки в элемент
TypeDataECSV str_to_element(string& dest, const string& str)
{
    string tempStr(str);
    trimstr(tempStr);
    size_t n_back = tempStr.length() - 1;

    if(tempStr[0] == '\"' && tempStr[n_back] == '\"')
    {
        tempStr.erase(n_back);
        tempStr.erase(0, 1);
        dest = tempStr;
        return TypeDataECSV::String;
    }
    else
    {
        dest = tempStr;
        return TypeDataECSV::Element;
    }
}

//Преобразование строки в элемент
vector<TypeDataECSV> str_to_element(vector<string>& vecstr)
{
    vector<TypeDataECSV> td;
    for(vector<string>::iterator iter = vecstr.begin(); iter != vecstr.end(); iter++)
    {
        td.push_back(str_to_element(*iter, *iter));
    }
    return td;
}

//Преобразование символов в нижний регистр
string& str_to_lower(string& str)
{
    for(string::iterator iter = str.begin(); iter != str.end(); iter++)
    {
        *iter = tolower(*iter);
    }
    return str;
}

//Удаление комментариев в строке
size_t delcomment(string& dest)
{
    size_t ind = dest.find("//", 0, 2);
    if(ind == 0)
    {
        dest = "";
    }
    else if(ind != std::string::npos)
    {
        dest = dest.substr(0, ind);
    }
    else
    {
        ind = dest.length();
    }
    return ind;
}

//Экранирование
void shield(string& dest, const string& str)
{
    const string SPEC("\";\\$/");
    dest = "";

    for(size_t ind = 0; ind < str.length(); ind++)
    {
        if(str[ind] == '\n')
        {
            dest.push_back('\\');
            dest.push_back('n');
        }
        else if(SPEC.find(str[ind]) != string::npos)
        {
            dest.push_back('\\');
        }
        dest.push_back(str[ind]);
    }
}

//Экранирование
void shield(vector<string>& vecstr)
{
    for(vector<string>::iterator iter = vecstr.begin(); iter != vecstr.end(); iter++)
    {
        shield(*iter, *iter);
    }
}

//Преобразование экранированных символов
void freeshield(string& dest)
{
    string str = dest;
    dest = "";
    size_t ind = 0;
    size_t n_str = str.length();

    while(ind < n_str)
    {
        if(str[ind] == '\\')
        {
            ind++;
            if(str[ind] == 'n') {dest.push_back('\n');}
            else {dest.push_back(str[ind]);}
        }
        else {dest.push_back(str[ind]);}
        ind++;
    }
}

//Преобразование экранированных символов
void freeshield(vector<string>& vecstr)
{
    for(vector<string>::iterator iter = vecstr.begin(); iter != vecstr.end(); iter++)
    {
        freeshield(*iter);
    }
}

//Преобразование строки в значение
void to_value(double& value, string& str_val)
{
    value = atof(str_val.c_str());
}

void to_value(float& value, string& str_val)
{
    value = atof(str_val.c_str());
}

void to_value(signed int& value, string& str_val)
{
    value = atoi(str_val.c_str());
}

void to_value(unsigned int& value, string& str_val)
{
    value = atol(str_val.c_str());
}

void to_value(long& value, string& str_val)
{
    value = atol(str_val.c_str());
}

void to_value(bool& value, string str_val)
{
    str_to_lower(str_val);
    if(str_val == "false" || str_val == "0") {value = false;}
    else {value = true;}
}

string to_vstring(bool& value)
{
    return (value ? "true" : "false");
}
