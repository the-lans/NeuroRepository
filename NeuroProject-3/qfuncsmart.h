#ifndef QFUNCSMART_H
#define QFUNCSMART_H

#include <vector>
#include <QString>
#include <QStringList>

using namespace std;

void split(QStringList& dest, const QString& str, const char* delim); //Разделение строки на подстроки по разделителю
void split_no_slash(QStringList& dest, const QString& str, const char* delim); //Разделение строки на подстроки по разделителю с учётом символа экранирования
void concat(QString& str, QStringList& dest, const char* delim); //Сбор строки из подстрок
void concat(QString& str, QString* dest, int len, const char* delim); //Сбор строки из подстрок

#endif // QFUNCSMART_H
