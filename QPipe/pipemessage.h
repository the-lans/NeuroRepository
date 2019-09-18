#ifndef PIPEMESSAGE_H
#define PIPEMESSAGE_H

#include "qfuncsmart.h"
#include <QString>
#include <QStringList>

struct PipeMessage
{
    PipeMessage();

    void deinit();
    void parsing(QString& str, QString& delim_data);
    QString toString(const QString& delim);
    string toStdString(const QString& delim);

    QString message; //Сообщение
    QString command; //Последняя команда
    QStringList param; //Параметры команды
};

#endif // PIPEMESSAGE_H
