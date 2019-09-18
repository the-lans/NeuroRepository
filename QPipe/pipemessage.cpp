#include "pipemessage.h"

PipeMessage::PipeMessage()
{
    this->deinit();
}

void PipeMessage::deinit()
{
    this->command = "";
    this->message = "";
    this->param.clear();
}

void PipeMessage::parsing(QString& str, QString& delim_data)
{
    QStringList vec;

    message = "";
    command = "";
    param.clear();

    split(vec, str, delim_data.toStdString().c_str());
    if(vec.size() > 0) {message = vec[0].trimmed();}
    if(vec.size() > 1) {command = vec[1].trimmed();}
    for(int ind = 2; ind < vec.size(); ind++)
    {
        param.push_back(vec[ind].trimmed());
    }
}

QString PipeMessage::toString(const QString& delim)
{
    QString strvec;
    QString str = message;

    concat(strvec, param, delim.toStdString().c_str());

    if(command != "")
    {
        str += delim + command;
        if(param.size() > 0) {str += delim + strvec;}
    }
    return str;
}

string PipeMessage::toStdString(const QString& delim)
{
    return this->toString(delim).toStdString();
}
