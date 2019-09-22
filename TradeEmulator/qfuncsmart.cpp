#include "qfuncsmart.h"

//Разделение строки на подстроки по разделителю
void split(QStringList& dest, const QString& str, const char* delim)
{
    dest = str.split(delim);
}

//Сбор строки из подстрок
void concat(QString& str, QStringList& dest, const char* delim)
{
    str = "";
    for(QStringList::iterator iter = dest.begin(); iter != dest.end(); iter++)
    {
        str.append(*iter);
        str.append(delim);
    }
    int len_delim = strlen(delim);
    if(str.length() > len_delim) {str.remove(str.length()-len_delim, len_delim);}
}

//Сбор строки из подстрок
void concat(QString& str, QString* dest, int len, const char* delim)
{
    str = "";
    for(int i = 0; i < len; i++)
    {
        str.append(dest[i]);
        str.append(delim);
    }
    int len_delim = strlen(delim);
    if(str.length() > len_delim) {str.remove(str.length()-len_delim, len_delim);}
}
