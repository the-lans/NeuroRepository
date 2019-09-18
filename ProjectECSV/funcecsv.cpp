#include "funcecsv.h"

//Чтение файла в строку
void readFile(string& str, const string& fileName)
{
    ifstream f(fileName);
    stringstream ss;
    ss << f.rdbuf();
    str = ss.str();
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

//Разделение строки на подстроки по разделителю
void split(NArray<string>& dest, const string& str, const char* delim)
{
    size_t cout_delim = strlen(delim);
    size_t ind1 = 0, ind2 = 0;
    string substr;

    while(ind1 < str.length() && ind2 != std::string::npos)
    {
        ind2 = str.find(delim, ind1, cout_delim);
        if(ind2 == std::string::npos) {ind2 = str.length();}
        substr = (ind1 == ind2 ? "" : str.substr(ind1, ind2 - ind1));
        dest.push(substr);
        ind1 = ind2 + cout_delim;
    }

    if(ind2 < str.length()) {dest.push("");}
}

void split(NArray<string>& dest, string str, NArray<string>& delim)
{
    for(int ind = 1; ind < delim.getLength(); ind++) {replacestr(str, delim[ind], delim[0]);}
    split(dest, str, delim[0].c_str());
}

void splitno(NArray<string>& dest, const string& str, const char* delim)
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
            dest.push(substr);
        }
        ind1 = ind2 + cout_delim;
    }
}

void splitno(NArray<string>& dest, string str, NArray<string>& delim)
{
    for(int ind = 1; ind < delim.getLength(); ind++) {replacestr(str, delim[ind], delim[0]);}
    splitno(dest, str, delim[0].c_str());
}

void concat(string& str, NArray<string>& dest, const char* delim)
{
    str = "";
    for(NArray<string>::iterator iter = dest.begin(); iter != dest.end(); iter++)
    {
        str.append(*iter);
        str.append(delim);
    }
    size_t len_delim = strlen(delim);
    if(str.length() > len_delim) {str.erase(str.length()-len_delim, len_delim);}
}
