#include "funcecsv.h"

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
