#include "funcsmart.h"

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

//Возвращает цельный путь
string getpath(string parent, string& field, const char sym)
{
    if(parent[0] == sym) {parent = parent.substr(1);}
    return parent + sym + field;
}

string getpath(string parent, const string& field, const char sym)
{
    if(parent[0] == sym) {parent = parent.substr(1);}
    return parent + sym + field;
}

//Разрешённое поле
bool resolvedfield(vector<string>* fieldsNotSave, string& parent, string& field)
{
    if(fieldsNotSave == nullptr) {return true;}
    bool bl = true;

    for(size_t ind = 0; bl && ind < fieldsNotSave->size(); ind++)
    {
        fieldsNotSave->at(ind);
        bl = !cmppath(getpath(parent, field), fieldsNotSave->at(ind), false);
    }

    //return fieldsNotSave == nullptr || find(fieldsNotSave->begin(), fieldsNotSave->end(), getpath(parent, field)) != fieldsNotSave->end();
    return bl;
}

bool resolvedfield(vector<string>* fieldsNotSave, string& parent, const string& field)
{
    if(fieldsNotSave == nullptr) {return true;}
    bool bl = true;

    for(size_t ind = 0; bl && ind < fieldsNotSave->size(); ind++)
    {
        fieldsNotSave->at(ind);
        bl = !cmppath(getpath(parent, field), fieldsNotSave->at(ind), false);
    }

    //return fieldsNotSave == nullptr || find(fieldsNotSave->begin(), fieldsNotSave->end(), getpath(parent, field)) != fieldsNotSave->end();
    return bl;
}

//Подстрока входит в строку
bool cmppath(const string& str, const string& strsub, bool acc, const char* sym)
{
    vector<string> path, subpath;

    split(path, str, sym);
    split(subpath, strsub, sym);
    if(path.size() > 0 && path[0] == "") {path.erase(path.begin());}
    if(subpath.size() > 0 && subpath[0] == "") {subpath.erase(subpath.begin());}

    return cmppath(path, subpath, acc);
}

bool cmppath(vector<string>& path, vector<string>& subpath, bool acc)
{
    bool bl = (!acc || path.size() == subpath.size());

    for(size_t i = 0; bl == true && i < subpath.size(); i++)
    {
        if(subpath[i] != path[i]) {bl = false;}
    }

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

void split(vector<string>& dest, string str, vector<string>& delim)
{
    for(size_t ind = 1; ind < delim.size(); ind++) {replacestr(str, delim[ind], delim[0]);}
    split(dest, str, delim[0].c_str());
}

void splitno(vector<string>& dest, const string& str, const char* delim)
{
    size_t cout_delim = strlen(delim);
    size_t ind1 = 0, ind2 = 0;
    string substr;

    while(ind1 < str.length() && ind2 != std::string::npos)
    {
        ind2 = str.find(delim, ind1, cout_delim);
        if(ind2 == std::string::npos) {ind2 = str.length();}
        if(ind1 != ind2)
        {
            substr = str.substr(ind1, ind2 - ind1);
            dest.push_back(substr);
        }
        ind1 = ind2 + cout_delim;
    }
}

void splitno(vector<string>& dest, string str, vector<string>& delim)
{
    for(size_t ind = 1; ind < delim.size(); ind++) {replacestr(str, delim[ind], delim[0]);}
    splitno(dest, str, delim[0].c_str());
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
    if(str.length() > len_delim) {str.erase(str.length()-len_delim, len_delim);}
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
    if(str.length() > len_delim) {str.erase(str.length()-len_delim, len_delim);}
}

//Замена подстроки
void replacestr(string& str, const string& src, const string& dst)
{
    size_t pos = str.find(src);
    while(pos != std::string::npos)
    {
        str.replace(pos, src.size(), dst);
        pos = str.find(src, pos);
    }
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

int trimBOM_utf8(string& s)
{
    const char space[4] = {(char)239, (char)187, (char)191, '\0'};
    if(s.find(space, 0, 3) == 0) {s.erase(0, 3); return 3;}
    return 0;
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

//Это число?
bool isnumber(const string& str)
{
    if(!isdigit(str[0]) && str[0] != '-' && str[0] != '.') {return false;}

    for(size_t ind = 1; ind < str.length(); ind++)
    {
        if(!isdigit(str[ind]) && str[ind] != '.') {return false;}
    }

    return true;
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

void to_value(string& value, string& str_val)
{
    value = str_val;
}

string to_string(string& val)
{
    return val;
}

string to_vstring(bool& value)
{
    return (value ? "true" : "false");
}
